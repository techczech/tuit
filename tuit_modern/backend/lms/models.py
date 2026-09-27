from django.db import models
from users.models import User

class ExerciseType(models.Model):
    short = models.CharField(max_length=12, blank=True, null=True)
    name = models.CharField(max_length=80, unique=True)
    
    def __str__(self):
        return self.name

class Exercise(models.Model):
    type = models.ForeignKey(ExerciseType, on_delete=models.CASCADE)
    name = models.CharField(max_length=80)
    description = models.CharField(max_length=255, blank=True, null=True)
    creator = models.ForeignKey(User, on_delete=models.SET_NULL, null=True)
    task = models.CharField(max_length=80, blank=True, null=True)
    story = models.TextField(blank=True, null=True)
    difficulty = models.IntegerField(default=0)
    options = models.CharField(max_length=255, blank=True, null=True)
    
    def __str__(self):
        return self.name

class QAndA(models.Model):
    exercise = models.ForeignKey(Exercise, on_delete=models.CASCADE, related_name='qanda_items')
    question = models.CharField(max_length=160, blank=True, null=True)
    answer = models.CharField(max_length=160, blank=True, null=True)

class Choose(models.Model):
    exercise = models.ForeignKey(Exercise, on_delete=models.CASCADE, related_name='choose_items')
    question = models.CharField(max_length=160, blank=True, null=True)
    choices = models.TextField(blank=True, null=True)
    correct = models.IntegerField(default=0)

class Match(models.Model):
    exercise = models.ForeignKey(Exercise, on_delete=models.CASCADE, related_name='match_items')
    first = models.CharField(max_length=160, blank=True, null=True)
    second = models.CharField(max_length=160, blank=True, null=True)

class ExerciseSet(models.Model):
    name = models.CharField(max_length=80)
    creator = models.ForeignKey(User, on_delete=models.SET_NULL, null=True)
    difficulty = models.IntegerField(default=0)
    
    def __str__(self):
        return self.name

class SetSection(models.Model):
    exercise_set = models.ForeignKey(ExerciseSet, on_delete=models.CASCADE, related_name='sections')
    name = models.CharField(max_length=50)
    seq = models.IntegerField(default=0)

class SetItem(models.Model):
    exercise_set = models.ForeignKey(ExerciseSet, on_delete=models.CASCADE, related_name='items')
    exercise = models.ForeignKey(Exercise, on_delete=models.CASCADE)
    section = models.ForeignKey(SetSection, on_delete=models.SET_NULL, null=True, blank=True)
    seq = models.IntegerField(default=0)

class Assignment(models.Model):
    description = models.CharField(max_length=200, blank=True, null=True)
    assigned_by = models.ForeignKey(User, on_delete=models.CASCADE, related_name='assignments_created')
    created_at = models.DateTimeField(auto_now_add=True)
    is_active = models.BooleanField(default=True)
    comment = models.TextField(blank=True, null=True)
    
class Answer(models.Model):
    exercise = models.ForeignKey(Exercise, on_delete=models.CASCADE)
    user = models.ForeignKey(User, on_delete=models.CASCADE)
    assignment = models.ForeignKey(Assignment, on_delete=models.SET_NULL, null=True, blank=True)
    points = models.FloatField(default=0)
    comment = models.TextField(blank=True, null=True)
    submitted_at = models.DateTimeField(auto_now_add=True)
