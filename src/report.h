#ifndef REPORT_H
#define REPORT_H

#include <stdio.h>
#include "grading.h"

#define REPORT_FILENAME "student_report.txt"

/* Prints a calculated student's report to a stream; returns false on output error. */
bool report_print(FILE *output, const Student *student, const GradeSummary *summary);

/* Replaces the report file; detects opening, writing, flushing and closing errors. */
bool report_save(const char *filename, const Student *student, const GradeSummary *summary);

#endif
