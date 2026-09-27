from django.contrib import admin
from django.urls import path, include
from rest_framework import routers

from cms.views import ArticleViewSet, ArticleCategoryViewSet
from lms.views import ExerciseViewSet, ExerciseSetViewSet
from dictionary.views import DictionaryViewSet, VocabularyViewSet

router = routers.DefaultRouter()
router.register(r'cms/articles', ArticleViewSet)
router.register(r'cms/categories', ArticleCategoryViewSet)
router.register(r'lms/exercises', ExerciseViewSet)
router.register(r'lms/sets', ExerciseSetViewSet)
router.register(r'dictionary/dicts', DictionaryViewSet)
router.register(r'dictionary/vocab', VocabularyViewSet, basename='vocabulary')

urlpatterns = [
    path('admin/', admin.site.urls),
    path('api/', include(router.urls)),
]
