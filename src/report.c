#include "report.h"
#include <string.h>

enum ReportLayout {
    MIN_SUBJECT_WIDTH = 7,
    SCORE_WIDTH = 6,
    GRADE_WIDTH = 5,
    COLUMN_SPACING = 2
};

static int subject_column_width(const Student *student)
{
    int width = MIN_SUBJECT_WIDTH;
    for (size_t index = 0; index < student->subject_count; index++) {
        int length = (int)strlen(student->subjects[index].name);
        if (length > width) width = length;
    }
    return width;
}

static void print_separator(FILE *output, int width)
{
    for (int index = 0; index < width; index++) fputc('-', output);
    fputc('\n', output);
}

static void print_subjects(FILE *output, const Student *student, int subject_width)
{
    fprintf(output, "%-*s%*s%*s%*s%*s\n",
            subject_width, "Subject", COLUMN_SPACING, "", SCORE_WIDTH, "Score",
            COLUMN_SPACING, "", GRADE_WIDTH, "Grade");
    print_separator(output, subject_width + SCORE_WIDTH + GRADE_WIDTH + COLUMN_SPACING * 2);

    for (size_t index = 0; index < student->subject_count; index++) {
        const Subject *subject = &student->subjects[index];
        /* Reserve one character of the score field for the percent sign. */
        fprintf(output, "%-*s%*s%*d%%%*s%*d\n",
                subject_width, subject->name, COLUMN_SPACING, "",
                SCORE_WIDTH - 1, subject->score, COLUMN_SPACING, "",
                GRADE_WIDTH, subject->grade);
    }
}

static void print_summary(FILE *output, const GradeSummary *summary)
{
    fprintf(output, "\nAverage Grade: %.2f\n", summary->average_grade);
    fprintf(output, "Average Score: %.2f%%\n", summary->average_score);
    fprintf(output, "Overall Grade: %d\n", summary->overall_grade);
}

bool report_print(FILE *output, const Student *student, const GradeSummary *summary)
{
    int subject_width;
    int table_width;

    if (output == NULL || student == NULL || summary == NULL ||
        student->subjects == NULL || student->subject_count == 0) return false;

    subject_width = subject_column_width(student);
    table_width = subject_width + SCORE_WIDTH + GRADE_WIDTH + COLUMN_SPACING * 2;
    print_separator(output, table_width);
    fprintf(output, "Student: %s\nSubjects: %zu\n", student->name, student->subject_count);
    print_separator(output, table_width);
    print_subjects(output, student, subject_width);
    print_summary(output, summary);
    print_separator(output, table_width);

    /* Flush so failures are detected even when the stream is buffered. */
    return fflush(output) == 0 && !ferror(output);
}

bool report_save(const char *filename, const Student *student, const GradeSummary *summary)
{
    FILE *file = fopen(filename, "w");
    bool success;

    if (file == NULL) {
        fprintf(stderr, "Could not open report file '%s' for writing.\n", filename);
        return false;
    }
    success = report_print(file, student, summary);
    /* Always close the file, including after a failed write. */
    if (fclose(file) == EOF) success = false;
    if (!success) {
        fprintf(stderr, "Could not finish writing report file '%s'.\n", filename);
    }
    return success;
}
