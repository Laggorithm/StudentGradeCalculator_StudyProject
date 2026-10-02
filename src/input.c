#include "input.h"
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_INPUT_CAPACITY 128

static void discard_line(FILE *input)
{
    int character;
    while ((character = fgetc(input)) != '\n' && character != EOF) {}
}

static void trim_whitespace(char *text)
{
    size_t start = 0;
    size_t length = strlen(text);

    while (start < length && isspace((unsigned char)text[start])) start++;
    while (length > start && isspace((unsigned char)text[length - 1])) length--;
    memmove(text, text + start, length - start);
    text[length - start] = '\0';
}

bool input_read_name(FILE *input, FILE *output,
                     char name[STUDENT_NAME_CAPACITY])
{
    /* Extra space lets us accept the maximum name plus CRLF without splitting it. */
    char line[STUDENT_NAME_CAPACITY + 2];

    for (;;) {
        size_t length;
        fprintf(output, "Please enter your name (maximum %d bytes): ",
                STUDENT_NAME_CAPACITY - 1);
        fflush(output);

        if (fgets(line, sizeof(line), input) == NULL) return false;

        length = strcspn(line, "\n");
        if (line[length] == '\n') {
            line[length] = '\0';
        } else if (!feof(input)) {
            /* Drain the rest so it cannot become the answer to the next prompt. */
            discard_line(input);
        }
        if (length > 0 && line[length - 1] == '\r') line[--length] = '\0';

        if (length >= STUDENT_NAME_CAPACITY) {
            fprintf(output, "Name is too long. Maximum length is %d bytes.\n",
                    STUDENT_NAME_CAPACITY - 1);
            continue;
        }

        trim_whitespace(line);
        if (line[0] == '\0') {
            fprintf(output, "Name cannot be empty. Please try again.\n");
            continue;
        }

        strcpy(name, line);
        return true;
    }
}

bool input_read_integer(FILE *input, FILE *output, const char *prompt,
                        int minimum, int maximum, int *value)
{
    char line[NUMBER_INPUT_CAPACITY];

    for (;;) {
        char *end;
        long number;

        fprintf(output, "%s", prompt);
        fflush(output);
        if (fgets(line, sizeof(line), input) == NULL) return false;
        if (strchr(line, '\n') == NULL && !feof(input)) {
            /* Reject a full buffer even when the valid-looking prefix is a number. */
            discard_line(input);
            fprintf(output, "Input is too long. Please enter a shorter number.\n");
            continue;
        }
        trim_whitespace(line);
        if (line[0] == '\0') {
            fprintf(output, "Input cannot be empty. Please enter a whole number.\n");
            continue;
        }

        errno = 0;
        number = strtol(line, &end, 10);
        if (end == line || *end != '\0') {
            fprintf(output, "Invalid input. Please enter a whole number.\n");
        } else if (errno == ERANGE || number < minimum || number > maximum) {
            fprintf(output, "Please enter a number between %d and %d.\n",
                    minimum, maximum);
        } else {
            *value = (int)number;
            return true;
        }
    }
}
