from django.db import models
from users.models import User

class ArticleCategory(models.Model):
    name = models.CharField(max_length=100)
    description = models.TextField(blank=True)
    
    def __str__(self):
        return self.name

class ArticleAuthor(models.Model):
    name = models.CharField(max_length=100)
    email = models.EmailField(max_length=100, blank=True)
    byline = models.TextField(blank=True)
    pic_url = models.CharField(max_length=100, blank=True, null=True)
    
    def __str__(self):
        return self.name

class Article(models.Model):
    title = models.CharField(max_length=200)
    description = models.TextField(blank=True)
    content = models.TextField()
    author = models.ForeignKey(ArticleAuthor, on_delete=models.SET_NULL, null=True, blank=True)
    categories = models.ManyToManyField(ArticleCategory, related_name='articles')
    date_created = models.DateField(blank=True, null=True)
    
    def __str__(self):
        return self.title
