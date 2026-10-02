#ifndef GRADING_H
#define GRADING_H

#include "student.h"

typedef struct {
    double average_grade;
    double average_score;
    int overall_grade;
} GradeSummary;

/* Applies the 0-5 scale; invalid scores return GRADE_NOT_ENTERED. */
int grading_from_score(double score);
/* Calculates grades only for a complete student; leaves data unchanged on failure. */
bool grading_calculate(Student *student, GradeSummary *summary);

#endif
