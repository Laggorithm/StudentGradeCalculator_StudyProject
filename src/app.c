#include "app.h"
#include "input.h"
#include <stdlib.h>

int app_run(FILE *input, FILE *output)
{
    char name[STUDENT_NAME_CAPACITY];

    fprintf(output, "Welcome to the Student Grade Calculator!\n");
    if (!input_read_name(input, output, name)) {
        fprintf(output, "Input ended or could not be read.\n");
        return EXIT_FAILURE;
    }

    fprintf(output, "Hello, %s!\n", name);
    return EXIT_SUCCESS;
}
