from django.contrib import admin
from .models import ArticleCategory, ArticleAuthor, Article

@admin.register(Article)
class ArticleAdmin(admin.ModelAdmin):
    list_display = ('title', 'author', 'date_created')
    search_fields = ('title', 'content')
    list_filter = ('categories', 'author')

admin.site.register(ArticleCategory)
admin.site.register(ArticleAuthor)
