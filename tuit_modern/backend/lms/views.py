from rest_framework import viewsets
from .models import Exercise, ExerciseSet
from .serializers import ExerciseSerializer, ExerciseSetSerializer

class ExerciseViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = Exercise.objects.all()
    serializer_class = ExerciseSerializer

class ExerciseSetViewSet(viewsets.ReadOnlyModelViewSet):
    queryset = ExerciseSet.objects.all()
    serializer_class = ExerciseSetSerializer
