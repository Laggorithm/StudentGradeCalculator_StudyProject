#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <stdio.h>
#include "student.h"

/* Reads a nonempty trimmed line; capacity includes the terminating null byte. */
bool input_read_text(FILE *input, FILE *output, const char *prompt,
                     char *text, size_t capacity);

/* Collects each subject's name and score; returns false on EOF/read error. */
bool input_read_subjects(FILE *input, FILE *output, Student *student);

/* Retries invalid names. Returns false on EOF/read error. */
bool input_read_name(FILE *input, FILE *output,
                     char name[STUDENT_NAME_CAPACITY]);

/* Retries invalid integers; leaves value unchanged if input ends or fails. */
bool input_read_integer(FILE *input, FILE *output, const char *prompt,
                        int minimum, int maximum, int *value);

#endif
