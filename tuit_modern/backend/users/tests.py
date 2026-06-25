from django.test import TestCase
from users.management.commands.migrate_legacy_tuit import parse_sql_values

class DataMigrationTests(TestCase):
    def test_parse_sql_values_simple(self):
        val_str = "(1, 'test', NULL), (2, 'hello', 'world')"
        records = parse_sql_values(val_str)
        self.assertEqual(len(records), 2)
        self.assertEqual(records[0], ('1', 'test', None))
        self.assertEqual(records[1], ('2', 'hello', 'world'))

    def test_parse_sql_values_with_escaped_quotes(self):
        val_str = "(3, 'test '' quote', 'val')"
        records = parse_sql_values(val_str)
        self.assertEqual(len(records), 1)
        self.assertEqual(records[0], ('3', "test ' quote", 'val'))

    def test_parse_sql_values_with_commas_in_string(self):
        val_str = "(4, 'hello, world', 'val')"
        records = parse_sql_values(val_str)
        self.assertEqual(len(records), 1)
        self.assertEqual(records[0], ('4', 'hello, world', 'val'))
