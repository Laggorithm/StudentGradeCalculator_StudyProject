#include "input.h"
#include <ctype.h>
#include <string.h>

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
