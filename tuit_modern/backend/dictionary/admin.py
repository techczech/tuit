from django.contrib import admin
from .models import Dictionary, Vocabulary

class VocabularyInline(admin.TabularInline):
    model = Vocabulary
    extra = 1

@admin.register(Dictionary)
class DictionaryAdmin(admin.ModelAdmin):
    list_display = ('name', 'description')

@admin.register(Vocabulary)
class VocabularyAdmin(admin.ModelAdmin):
    list_display = ('czech', 'english', 'dictionary')
    list_filter = ('dictionary',)
    search_fields = ('czech', 'english', 'latin', 'german')
