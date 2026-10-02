#ifndef STUDENT_H
#define STUDENT_H

#include <stdbool.h>
#include <stddef.h>

#define STUDENT_NAME_CAPACITY 128
#define SUBJECT_NAME_CAPACITY 128
#define MIN_SUBJECT_COUNT 1
#define MAX_SUBJECT_COUNT 1000
#define GRADE_NOT_ENTERED (-1)

typedef struct {
    char name[SUBJECT_NAME_CAPACITY];
    int score;
    int grade;
} Subject;

typedef struct {
    char name[STUDENT_NAME_CAPACITY];
    Subject *subjects;
    size_t subject_count;
} Student;

/* Student must be zero-initialized and must not own an existing subject array. */
bool student_create(Student *student, const char *name, size_t subject_count);
/* Frees owned memory and resets the student; safe to call more than once. */
void student_destroy(Student *student);

#endif
