#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <stdio.h>

/* Capacity includes the terminating null byte; the limit is measured in bytes. */
#define STUDENT_NAME_CAPACITY 128

/* Retries invalid names. Returns false on EOF/read error without changing name. */
bool input_read_name(FILE *input, FILE *output,
                     char name[STUDENT_NAME_CAPACITY]);

#endif
