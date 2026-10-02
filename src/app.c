#include "app.h"
#include "input.h"
#include <stdlib.h>

int app_run(FILE *input, FILE *output)
{
    char name[STUDENT_NAME_CAPACITY];
    char prompt[STUDENT_NAME_CAPACITY];
    int subject_count;
    Student student = {0};

    fprintf(output, "Welcome to the Student Grade Calculator!\n");
    if (!input_read_name(input, output, name)) {
        fprintf(output, "Input ended or could not be read.\n");
        return EXIT_FAILURE;
    }

    fprintf(output, "Hello, %s!\n", name);
    snprintf(prompt, sizeof(prompt),
             "How many subjects do you want to calculate grades for (%d-%d)? ",
             MIN_SUBJECT_COUNT, MAX_SUBJECT_COUNT);
    if (!input_read_integer(input, output, prompt, MIN_SUBJECT_COUNT,
                            MAX_SUBJECT_COUNT, &subject_count)) {
        fprintf(output, "Input ended or could not be read.\n");
        return EXIT_FAILURE;
    }
    if (!student_create(&student, name, (size_t)subject_count)) {
        fprintf(output, "Could not allocate memory for the student's subjects.\n");
        return EXIT_FAILURE;
    }

    fprintf(output, "Student: %s\nSubjects: %zu\n",
            student.name, student.subject_count);
    student_destroy(&student);
    return EXIT_SUCCESS;
}
