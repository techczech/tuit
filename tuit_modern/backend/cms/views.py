from rest_framework import viewsets
from .models import Article, ArticleCategory
from .serializers import ArticleSerializer, ArticleCategorySerializer

class ArticleViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = Article.objects.all().order_by('-date_created')
    serializer_class = ArticleSerializer

class ArticleCategoryViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = ArticleCategory.objects.all()
    serializer_class = ArticleCategorySerializer
