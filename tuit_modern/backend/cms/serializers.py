from rest_framework import serializers
from .models import ArticleCategory, ArticleAuthor, Article

class ArticleCategorySerializer(serializers.ModelSerializer):
    class Meta:
        model = ArticleCategory
        fields = '__all__'

class ArticleAuthorSerializer(serializers.ModelSerializer):
    class Meta:
        model = ArticleAuthor
        fields = '__all__'

class ArticleSerializer(serializers.ModelSerializer):
    author = ArticleAuthorSerializer(read_only=True)
    categories = ArticleCategorySerializer(many=True, read_only=True)
    
    class Meta:
        model = Article
        fields = '__all__'
