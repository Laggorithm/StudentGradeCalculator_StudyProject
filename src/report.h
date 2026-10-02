#ifndef REPORT_H
#define REPORT_H

#include <stdio.h>
#include "grading.h"

/* Prints a calculated student's report to a stream; returns false on output error. */
bool report_print(FILE *output, const Student *student, const GradeSummary *summary);

#endif
