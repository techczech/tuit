from django.test import TestCase
from users.models import User
from lms.models import ExerciseType, Exercise, Assignment, Answer

class LMSBusinessLogicTests(TestCase):
    def setUp(self):
        self.user = User.objects.create(username='student1', email='student1@example.com')
        self.teacher = User.objects.create(username='teacher1', email='teacher1@example.com', is_admin=True)
        self.exercise_type = ExerciseType.objects.create(name='Multiple Choice', short='MC')
        self.exercise = Exercise.objects.create(
            type=self.exercise_type,
            name='Test Exercise',
            difficulty=1,
            creator=self.teacher
        )
        self.assignment = Assignment.objects.create(
            description='Test Assignment',
            assigned_by=self.teacher,
            is_active=True
        )

    def test_answer_submission(self):
        """Test submitting an answer to an assignment"""
        answer = Answer.objects.create(
            exercise=self.exercise,
            user=self.user,
            assignment=self.assignment,
            points=10.0,
            comment='Good job'
        )
        self.assertEqual(Answer.objects.count(), 1)
        self.assertEqual(answer.points, 10.0)
        self.assertEqual(answer.user.username, 'student1')

    def test_assignment_active_status(self):
        """Test assignment active status"""
        self.assertTrue(self.assignment.is_active)
        self.assignment.is_active = False
        self.assignment.save()
        self.assertFalse(self.assignment.is_active)
