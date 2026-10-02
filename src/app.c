#include "app.h"
#include "input.h"
#include "grading.h"
#include <stdlib.h>

int app_run(FILE *input, FILE *output)
{
    char name[STUDENT_NAME_CAPACITY];
    char prompt[STUDENT_NAME_CAPACITY];
    int subject_count;
    Student student = {0};
    GradeSummary summary;

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

    if (!input_read_subjects(input, output, &student)) {
        fprintf(output, "Input ended before all subjects were entered.\n");
        student_destroy(&student);
        return EXIT_FAILURE;
    }

    if (!grading_calculate(&student, &summary)) {
        fprintf(output, "Could not calculate grades: subject data is incomplete or invalid.\n");
        student_destroy(&student);
        return EXIT_FAILURE;
    }

    fprintf(output, "\nStudent: %s\nSubjects: %zu\n",
            student.name, student.subject_count);
    for (size_t index = 0; index < student.subject_count; index++) {
        fprintf(output, "%s: %d%%, grade %d\n", student.subjects[index].name,
                student.subjects[index].score, student.subjects[index].grade);
    }
    fprintf(output, "\nAverage Grade: %.2f\nAverage Score: %.2f%%\nOverall Grade: %d\n",
            summary.average_grade, summary.average_score, summary.overall_grade);
    student_destroy(&student);
    return EXIT_SUCCESS;
}
