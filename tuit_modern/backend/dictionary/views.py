from rest_framework import viewsets
from .models import Dictionary, Vocabulary
from .serializers import DictionarySerializer, VocabularySerializer

class DictionaryViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = Dictionary.objects.all()
    serializer_class = DictionarySerializer

class VocabularyViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = Vocabulary.objects.all()
    serializer_class = VocabularySerializer

    def get_queryset(self):
        queryset = super().get_queryset()
        dictionary_id = self.request.query_params.get('dictionary', None)
        if dictionary_id is not None:
            queryset = queryset.filter(dictionary_id=dictionary_id)
        return queryset
