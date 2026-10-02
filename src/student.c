#include "student.h"
#include <stdlib.h>
#include <string.h>

bool student_create(Student *student, const char *name, size_t subject_count)
{
    Subject *subjects;

    if (student == NULL || name == NULL || student->subjects != NULL ||
        name[0] == '\0' || strlen(name) >= STUDENT_NAME_CAPACITY ||
        subject_count < MIN_SUBJECT_COUNT || subject_count > MAX_SUBJECT_COUNT) {
        return false;
    }

    subjects = calloc(subject_count, sizeof(*subjects));
    if (subjects == NULL) return false;

    /* Empty names and explicit markers distinguish unused subjects from zero scores. */
    for (size_t index = 0; index < subject_count; index++) {
        subjects[index].score = GRADE_NOT_ENTERED;
        subjects[index].grade = GRADE_NOT_ENTERED;
    }

    strcpy(student->name, name);
    student->subjects = subjects;
    student->subject_count = subject_count;
    return true;
}

void student_destroy(Student *student)
{
    if (student == NULL) return;
    free(student->subjects);
    *student = (Student){0};
}
