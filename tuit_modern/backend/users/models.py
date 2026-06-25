from django.contrib.auth.models import AbstractUser
from django.db import models

class User(AbstractUser):
    name = models.CharField(max_length=100, blank=True, null=True)
    send_note = models.BooleanField(default=False)
    default_teacher = models.IntegerField(default=0)
    is_admin = models.BooleanField(default=False)
    cookie = models.CharField(max_length=32, blank=True, null=True)
    ip_address = models.GenericIPAddressField(blank=True, null=True)
    expiry = models.DateField(blank=True, null=True)
    logins_count = models.IntegerField(default=0)
    
    def __str__(self):
        return self.username
