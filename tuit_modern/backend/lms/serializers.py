from rest_framework import serializers
from .models import ExerciseType, Exercise, QAndA, Choose, Match, ExerciseSet, SetSection

class ExerciseTypeSerializer(serializers.ModelSerializer):
    class Meta:
        model = ExerciseType
        fields = '__all__'

class QAndASerializer(serializers.ModelSerializer):
    class Meta:
        model = QAndA
        fields = '__all__'

class ChooseSerializer(serializers.ModelSerializer):
    class Meta:
        model = Choose
        fields = '__all__'

class MatchSerializer(serializers.ModelSerializer):
    class Meta:
        model = Match
        fields = '__all__'

class ExerciseSerializer(serializers.ModelSerializer):
    type = ExerciseTypeSerializer(read_only=True)
    qanda_items = QAndASerializer(many=True, read_only=True)
    choose_items = ChooseSerializer(many=True, read_only=True)
    match_items = MatchSerializer(many=True, read_only=True)

    class Meta:
        model = Exercise
        fields = '__all__'

class ExerciseSetSerializer(serializers.ModelSerializer):
    class Meta:
        model = ExerciseSet
        fields = '__all__'
