#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <stdio.h>
#include "student.h"

/* Retries invalid names. Returns false on EOF/read error without changing name. */
bool input_read_name(FILE *input, FILE *output,
                     char name[STUDENT_NAME_CAPACITY]);

/* Retries invalid integers; leaves value unchanged if input ends or fails. */
bool input_read_integer(FILE *input, FILE *output, const char *prompt,
                        int minimum, int maximum, int *value);

#endif
