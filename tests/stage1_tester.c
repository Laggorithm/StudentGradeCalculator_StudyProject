#include "app.h"
#include "input.h"
#include <stdlib.h>
#include <string.h>

static int test_name(const char *label, const char *data,
                     const char *expected, const char *message)
{
    FILE *input = tmpfile();
    FILE *output = tmpfile();
    char name[STUDENT_NAME_CAPACITY] = "unchanged";
    char transcript[4096];
    bool result;
    int passed;
    size_t length;

    if (input == NULL || output == NULL) {
        if (input != NULL) fclose(input);
        if (output != NULL) fclose(output);
        fprintf(stderr, "FAIL: %s (cannot create temporary files)\n", label);
        return 1;
    }
    fputs(data, input);
    rewind(input);
    result = input_read_name(input, output, name);
    rewind(output);
    length = fread(transcript, 1, sizeof(transcript) - 1, output);
    transcript[length] = '\0';
    passed = expected != NULL
        ? result && strcmp(name, expected) == 0
        : !result && strcmp(name, "unchanged") == 0;
    if (message != NULL && strstr(transcript, message) == NULL) passed = 0;
    fclose(input);
    fclose(output);
    printf("%s: %s\n", passed ? "PASS" : "FAIL", label);
    return !passed;
}

static int test_app(void)
{
    FILE *input = tmpfile();
    FILE *output = tmpfile();
    char transcript[1024];
    int result;
    int passed;
    size_t length;
    if (input == NULL || output == NULL) {
        if (input != NULL) fclose(input);
        if (output != NULL) fclose(output);
        return 1;
    }
    fputs("John Deere\n", input);
    rewind(input);
    result = app_run(input, output);
    rewind(output);
    length = fread(transcript, 1, sizeof(transcript) - 1, output);
    transcript[length] = '\0';
    passed = result == EXIT_SUCCESS &&
        strstr(transcript, "Welcome to the Student Grade Calculator!") != NULL &&
        strstr(transcript, "Hello, John Deere!") != NULL;
    rewind(input);
    while (fgetc(input) != EOF) {}
    passed = passed && app_run(input, output) == EXIT_FAILURE;
    fclose(input);
    fclose(output);
    printf("%s: application greeting and EOF exit\n", passed ? "PASS" : "FAIL");
    return !passed;
}

int main(void)
{
    int failures = 0;
    char maximum[STUDENT_NAME_CAPACITY];
    char boundary_input[STUDENT_NAME_CAPACITY + 3];
    char oversized[STUDENT_NAME_CAPACITY * 4];
    memset(maximum, 'A', sizeof(maximum) - 1);
    maximum[sizeof(maximum) - 1] = '\0';
    snprintf(boundary_input, sizeof(boundary_input), "%s\r\n", maximum);
    memset(oversized, 'B', sizeof(oversized) - 1);
    oversized[sizeof(oversized) - 1] = '\0';
    strcpy(oversized + sizeof(oversized) - 9, "\nAlice\n");

    failures += test_name("name with spaces", "John Deere\n", "John Deere", NULL);
    failures += test_name("trim edges", "  John Deere \t\n", "John Deere", NULL);
    failures += test_name("empty then valid", "\nAlice\n", "Alice", "cannot be empty");
    failures += test_name("whitespace then valid", " \t\nAlice\n", "Alice", "cannot be empty");
    failures += test_name("maximum length with CRLF", boundary_input, maximum, NULL);
    failures += test_name("oversized then valid", oversized, "Alice", "too long");
    failures += test_name("last line without newline", "Alice", "Alice", NULL);
    failures += test_name("maximum length at EOF", maximum, maximum, NULL);
    failures += test_name("immediate EOF", "", NULL, NULL);
    failures += test_name("empty then EOF", "\n", NULL, "cannot be empty");
    failures += test_app();
    printf("\nStage 1: %s\n", failures == 0 ? "all tests passed" : "tests failed");
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
