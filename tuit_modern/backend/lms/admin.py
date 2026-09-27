from django.contrib import admin
from .models import ExerciseType, Exercise, QAndA, Choose, Match, ExerciseSet, SetSection, Assignment, Answer

class QAndAInline(admin.TabularInline):
    model = QAndA
    extra = 1

class ChooseInline(admin.TabularInline):
    model = Choose
    extra = 1

class MatchInline(admin.TabularInline):
    model = Match
    extra = 1

@admin.register(Exercise)
class ExerciseAdmin(admin.ModelAdmin):
    list_display = ('name', 'type', 'difficulty', 'creator')
    list_filter = ('type', 'difficulty')
    search_fields = ('name', 'description', 'task')
    inlines = [QAndAInline, ChooseInline, MatchInline]

class SetSectionInline(admin.TabularInline):
    model = SetSection
    extra = 1

@admin.register(ExerciseSet)
class ExerciseSetAdmin(admin.ModelAdmin):
    list_display = ('name', 'creator', 'difficulty')
    inlines = [SetSectionInline]

admin.site.register(ExerciseType)
admin.site.register(Assignment)
admin.site.register(Answer)
