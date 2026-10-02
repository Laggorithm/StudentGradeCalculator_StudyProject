#include "input.h"
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_INPUT_CAPACITY 128

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
    char prompt[STUDENT_NAME_CAPACITY];
    snprintf(prompt, sizeof(prompt), "Please enter your name (maximum %d bytes): ",
             STUDENT_NAME_CAPACITY - 1);
    return input_read_text(input, output, prompt, name, STUDENT_NAME_CAPACITY);
}

bool input_read_text(FILE *input, FILE *output, const char *prompt,
                     char *text, size_t capacity)
{
    if (capacity < 2) return false;

    for (;;) {
        size_t length = 0;
        int character;
        bool too_long = false;
        bool invalid_character = false;
        fprintf(output, "%s", prompt);
        fflush(output);
        /* Consume the entire line, even after overflow, keeping the next prompt clean. */
        while ((character = fgetc(input)) != '\n' && character != EOF) {
            if (character == '\r') continue;
            if (character == '\0') invalid_character = true;
            if (length < capacity - 1) text[length++] = (char)character;
            else too_long = true;
        }
        if (ferror(input) || (character == EOF && length == 0 && !too_long)) return false;
        text[length] = '\0';
        if (too_long) {
            fprintf(output, "Input is too long. Maximum length is %zu bytes.\n", capacity - 1);
            continue;
        }
        if (invalid_character) {
            fprintf(output, "Input contains an invalid character. Please try again.\n");
            continue;
        }
        trim_whitespace(text);
        if (text[0] == '\0') {
            fprintf(output, "Input cannot be empty. Please try again.\n");
            continue;
        }
        /* Reject embedded control characters that could disrupt the report columns. */
        for (size_t index = 0; text[index] != '\0'; index++) {
            if (iscntrl((unsigned char)text[index])) invalid_character = true;
        }
        if (invalid_character) {
            fprintf(output, "Input contains an invalid character. Please try again.\n");
            continue;
        }
        return true;
    }
}

bool input_read_subjects(FILE *input, FILE *output, Student *student)
{
    char prompt[SUBJECT_NAME_CAPACITY + NUMBER_INPUT_CAPACITY];

    for (size_t index = 0; index < student->subject_count; index++) {
        Subject *subject = &student->subjects[index];
        snprintf(prompt, sizeof(prompt), "\nEnter subject %zu name: ", index + 1);
        if (!input_read_text(input, output, prompt, subject->name, sizeof(subject->name))) {
            return false;
        }
        snprintf(prompt, sizeof(prompt), "Enter your score for %s (%d-%d): ",
                 subject->name, MIN_SCORE, MAX_SCORE);
        if (!input_read_integer(input, output, prompt, MIN_SCORE, MAX_SCORE, &subject->score)) {
            return false;
        }
    }
    return true;
}

bool input_read_integer(FILE *input, FILE *output, const char *prompt,
                        int minimum, int maximum, int *value)
{
    char line[NUMBER_INPUT_CAPACITY];

    for (;;) {
        char *end;
        long number;

        if (!input_read_text(input, output, prompt, line, sizeof(line))) return false;

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
