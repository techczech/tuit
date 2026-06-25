from django.db import models

class Dictionary(models.Model):
    name = models.CharField(max_length=50)
    description = models.TextField(blank=True, null=True)
    
    def __str__(self):
        return self.name

class Vocabulary(models.Model):
    dictionary = models.ForeignKey(Dictionary, on_delete=models.CASCADE, related_name='words')
    czech = models.CharField(max_length=100, blank=True, null=True)
    english = models.CharField(max_length=100, blank=True, null=True)
    latin = models.CharField(max_length=100, blank=True, null=True)
    german = models.CharField(max_length=100, blank=True, null=True)
    slovak = models.CharField(max_length=100, blank=True, null=True)
    note = models.CharField(max_length=255, blank=True, null=True)
    
    def __str__(self):
        return f"{self.czech} - {self.english}"
