import os
import re
import datetime
from django.core.management.base import BaseCommand
from django.conf import settings
from django.db import transaction

from users.models import User
from cms.models import ArticleCategory, ArticleAuthor, Article
from lms.models import ExerciseType, Exercise, QAndA, Choose, Match, ExerciseSet, SetSection, SetItem, Assignment, Answer
from dictionary.models import Dictionary, Vocabulary

def parse_sql_values(val_str):
    """
    Parses a string like `(1, 'abc', NULL), (2, 'def', 'x')`
    Returns a list of tuples.
    """
    records = []
    in_string = False
    escape = False
    current_val = []
    current_record = []
    
    i = 0
    while i < len(val_str):
        c = val_str[i]
        
        if escape:
            current_val.append(c)
            escape = False
        elif c == '\\':
            escape = True
        elif c == "'":
            if in_string and i + 1 < len(val_str) and val_str[i+1] == "'":
                # escaped quote
                current_val.append("'")
                i += 1
            else:
                in_string = not in_string
        elif not in_string:
            if c == '(':
                current_record = []
                current_val = []
            elif c == ',':
                if current_record is not None:
                    val = "".join(current_val).strip()
                    if val.upper() == 'NULL':
                        current_record.append(None)
                    else:
                        current_record.append(val)
                    current_val = []
            elif c == ')':
                if current_record is not None:
                    val = "".join(current_val).strip()
                    if val.upper() == 'NULL':
                        current_record.append(None)
                    else:
                        current_record.append(val)
                    records.append(tuple(current_record))
                    current_record = None
            else:
                current_val.append(c)
        else:
            current_val.append(c)
            
        i += 1
        
    return records


class Command(BaseCommand):
    help = 'Migrates data from legacy .sql files to modern Django models'

    def handle(self, *args, **options):
        self.stdout.write("Starting data migration from legacy SQL files...")
        
        data_dir = os.path.join(settings.BASE_DIR.parent, 'legacy_data')
        tuit_sql = os.path.join(data_dir, 'tuit.sql')
        
        if not os.path.exists(tuit_sql):
            self.stdout.write(self.style.ERROR(f'Legacy SQL file not found: {tuit_sql}'))
            return
            
        self.stdout.write("Parsing SQL dump...")
        
        table_data = {}
        current_table = None
        current_values_str = None
        
        # Read the file and extract INSERT statements
        with open(tuit_sql, 'r', encoding='utf-8', errors='ignore') as f:
            for line in f:
                line = line.strip()
                if line.startswith("INSERT INTO"):
                    match = re.match(r"INSERT INTO `(\w+)`[^\)]*\)\s*VALUES\s*(.*)", line, re.IGNORECASE)
                    if match:
                        current_table = match.group(1)
                        if current_table not in table_data:
                            table_data[current_table] = []
                        val_part = match.group(2)
                        current_values_str = val_part
                        if val_part.endswith(";"):
                            # Parse immediately
                            table_data[current_table].extend(parse_sql_values(current_values_str))
                            current_table = None
                            current_values_str = None
                        continue
                
                if current_table and current_values_str is not None:
                    current_values_str += " " + line
                    if line.endswith(";"):
                        table_data[current_table].extend(parse_sql_values(current_values_str))
                        current_table = None
                        current_values_str = None

        self.stdout.write(self.style.SUCCESS(f"Parsed data for {len(table_data)} tables."))
        for t, rows in table_data.items():
            self.stdout.write(f" - {t}: {len(rows)} rows")
            
        self.stdout.write("Migrating data to Django models...")
        
        with transaction.atomic():
            # Migrate Users
            if 'users' in table_data:
                self.stdout.write("Migrating Users...")
                for row in table_data['users']:
                    # id, login, passwd, name, email, send_note, default_teacher, admin, cookie, ip, last_change, expiry, logins, last_login
                    try:
                        uid = int(row[0])
                        username = row[1]
                        name = row[3]
                        email = row[4]
                        
                        user, created = User.objects.get_or_create(
                            id=uid,
                            defaults={
                                'username': username or f'user_{uid}',
                                'name': name,
                                'email': email,
                            }
                        )
                        if created:
                            user.set_unusable_password()
                            user.save()
                    except Exception as e:
                        pass
                        
            # Migrate Dictionary
            if 'dicts' in table_data:
                self.stdout.write("Migrating Dictionaries...")
                for row in table_data['dicts']:
                    try:
                        did = int(row[0])
                        dname = row[1]
                        ddesc = row[2]
                        Dictionary.objects.get_or_create(id=did, defaults={'name': dname, 'description': ddesc})
                    except Exception: pass
                    
            if 'vocab' in table_data:
                self.stdout.write("Migrating Vocabulary...")
                for row in table_data['vocab']:
                    try:
                        vid = int(row[0])
                        did = int(row[1])
                        cz = row[2]
                        en = row[3]
                        la = row[4]
                        ge = row[5]
                        sl = row[6]
                        note = row[7]
                        
                        # Only import if dictionary exists
                        if Dictionary.objects.filter(id=did).exists():
                            Vocabulary.objects.get_or_create(
                                id=vid,
                                defaults={
                                    'dictionary_id': did,
                                    'czech': cz,
                                    'english': en,
                                    'latin': la,
                                    'german': ge,
                                    'slovak': sl,
                                    'note': note
                                }
                            )
                    except Exception: pass
            
            # Migrate CMS
            if 'article_cats' in table_data:
                self.stdout.write("Migrating Article Categories...")
                for row in table_data['article_cats']:
                    try:
                        cid = int(row[0])
                        cname = row[1]
                        cdesc = row[2]
                        ArticleCategory.objects.get_or_create(id=cid, defaults={'name': cname, 'description': cdesc})
                    except Exception: pass
                    
            if 'article_authors' in table_data:
                self.stdout.write("Migrating Article Authors...")
                for row in table_data['article_authors']:
                    try:
                        aid = int(row[0])
                        name = row[1]
                        email = row[2]
                        byline = row[3]
                        ArticleAuthor.objects.get_or_create(id=aid, defaults={'name': name, 'email': email, 'byline': byline})
                    except Exception: pass
                    
            if 'articles' in table_data:
                self.stdout.write("Migrating Articles...")
                for row in table_data['articles']:
                    try:
                        aid = int(row[0])
                        desc = row[1]
                        title = row[2]
                        content = row[3]
                        author_id = int(row[4]) if row[4] else None
                        
                        # Author might not exist
                        if author_id and not ArticleAuthor.objects.filter(id=author_id).exists():
                            author_id = None
                            
                        Article.objects.get_or_create(
                            id=aid,
                            defaults={
                                'title': title,
                                'description': desc,
                                'content': content,
                                'author_id': author_id,
                            }
                        )
                    except Exception: pass

            # Migrate LMS - Exercise Types
            if 'exercise_types' in table_data:
                self.stdout.write("Migrating Exercise Types...")
                for row in table_data['exercise_types']:
                    try:
                        tid = int(row[0])
                        short = row[1]
                        name = row[2]
                        ExerciseType.objects.get_or_create(id=tid, defaults={'short': short, 'name': name})
                    except Exception: pass

            # Migrate LMS - Exercises
            if 'exercises' in table_data:
                self.stdout.write("Migrating Exercises...")
                for row in table_data['exercises']:
                    try:
                        eid = int(row[0])
                        etype = int(row[1])
                        name = row[2]
                        desc = row[3]
                        creator_id = int(row[4])
                        task = row[5]
                        story = row[6]
                        diff = int(row[7]) if row[7] else 0
                        opts = row[8]
                        
                        if ExerciseType.objects.filter(id=etype).exists() and User.objects.filter(id=creator_id).exists():
                            Exercise.objects.get_or_create(
                                id=eid,
                                defaults={
                                    'type_id': etype,
                                    'name': name,
                                    'description': desc,
                                    'creator_id': creator_id,
                                    'task': task,
                                    'story': story,
                                    'difficulty': diff,
                                    'options': opts
                                }
                            )
                    except Exception: pass

            if 'ex_qanda' in table_data:
                self.stdout.write("Migrating QAndA...")
                for row in table_data['ex_qanda']:
                    try:
                        qa_id = int(row[0])
                        ex_id = int(row[1])
                        question = row[2]
                        answer = row[3]
                        if Exercise.objects.filter(id=ex_id).exists():
                            QAndA.objects.get_or_create(id=qa_id, defaults={'exercise_id': ex_id, 'question': question, 'answer': answer})
                    except Exception: pass

            if 'ex_choose' in table_data:
                self.stdout.write("Migrating Choose...")
                for row in table_data['ex_choose']:
                    try:
                        ch_id = int(row[0])
                        ex_id = int(row[1])
                        question = row[2]
                        choices = row[3]
                        correct = int(row[4]) if row[4] else 0
                        if Exercise.objects.filter(id=ex_id).exists():
                            Choose.objects.get_or_create(id=ch_id, defaults={'exercise_id': ex_id, 'question': question, 'choices': choices, 'correct': correct})
                    except Exception: pass

            if 'ex_match' in table_data:
                self.stdout.write("Migrating Match...")
                for row in table_data['ex_match']:
                    try:
                        ma_id = int(row[0])
                        ex_id = int(row[1])
                        first = row[2]
                        second = row[3]
                        if Exercise.objects.filter(id=ex_id).exists():
                            Match.objects.get_or_create(id=ma_id, defaults={'exercise_id': ex_id, 'first': first, 'second': second})
                    except Exception: pass

            if 'sets' in table_data:
                self.stdout.write("Migrating Exercise Sets...")
                for row in table_data['sets']:
                    try:
                        set_id = int(row[0])
                        set_name = row[2]
                        set_creator = int(row[3])
                        set_diff = int(row[4]) if row[4] else 0
                        if User.objects.filter(id=set_creator).exists():
                            ExerciseSet.objects.get_or_create(id=set_id, defaults={'name': set_name, 'creator_id': set_creator, 'difficulty': set_diff})
                    except Exception: pass

            if 'set_sections' in table_data:
                self.stdout.write("Migrating Set Sections...")
                for row in table_data['set_sections']:
                    try:
                        ss_id = int(row[0])
                        set_id = int(row[1])
                        ss_name = row[2]
                        ss_seq = int(row[3]) if row[3] else 0
                        if ExerciseSet.objects.filter(id=set_id).exists():
                            SetSection.objects.get_or_create(id=ss_id, defaults={'exercise_set_id': set_id, 'name': ss_name, 'seq': ss_seq})
                    except Exception: pass

            if 'exercises_sets' in table_data:
                self.stdout.write("Migrating Set Items...")
                for row in table_data['exercises_sets']:
                    try:
                        se_id = int(row[0])
                        ex_id = int(row[1])
                        set_id = int(row[2])
                        ss_id = int(row[3]) if row[3] else None
                        seq = int(row[4]) if row[4] else 0
                        if Exercise.objects.filter(id=ex_id).exists() and ExerciseSet.objects.filter(id=set_id).exists():
                            # Only set ss_id if that section exists
                            if ss_id and not SetSection.objects.filter(id=ss_id).exists():
                                ss_id = None
                            SetItem.objects.get_or_create(id=se_id, defaults={'exercise_id': ex_id, 'exercise_set_id': set_id, 'section_id': ss_id, 'seq': seq})
                    except Exception: pass

            if 'assignments' in table_data:
                self.stdout.write("Migrating Assignments...")
                for row in table_data['assignments']:
                    try:
                        as_id = int(row[0])
                        desc = row[1]
                        # as_task_type, as_on, as_on_after, as_modifier, as_limit, as_who_type, as_by...
                        as_by = int(row[8])
                        as_active = bool(int(row[11])) if row[11] else True
                        as_comment = row[12]
                        if User.objects.filter(id=as_by).exists():
                            Assignment.objects.get_or_create(id=as_id, defaults={'description': desc, 'assigned_by_id': as_by, 'is_active': as_active, 'comment': as_comment})
                    except Exception: pass

            if 'answers' in table_data:
                self.stdout.write("Migrating Answers...")
                for row in table_data['answers']:
                    try:
                        an_id = int(row[0])
                        ex_id = int(row[1])
                        who_id = int(row[2])
                        as_id = int(row[3]) if row[3] else None
                        points = float(row[6]) if row[6] else 0.0
                        comment = row[7]
                        
                        if Exercise.objects.filter(id=ex_id).exists() and User.objects.filter(id=who_id).exists():
                            if as_id and not Assignment.objects.filter(id=as_id).exists():
                                as_id = None
                            Answer.objects.get_or_create(id=an_id, defaults={'exercise_id': ex_id, 'user_id': who_id, 'assignment_id': as_id, 'points': points, 'comment': comment})
                    except Exception: pass


        self.stdout.write(self.style.SUCCESS('Successfully migrated legacy data to modern Django database!'))
