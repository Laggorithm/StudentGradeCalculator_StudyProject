/* Final module checks: lifecycle, invalid data, grade boundaries and write errors. */
#include "grading.h"
#include "report.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define REQUIRE(condition) do { if (!(condition)) { \
    fprintf(stderr, "FAIL at line %d: %s\n", __LINE__, #condition); \
    return EXIT_FAILURE; } } while (0)

int main(void)
{
    Student student = {0};
    GradeSummary summary = {0};
    FILE *read_only;

    REQUIRE(grading_from_score(-1) == GRADE_NOT_ENTERED);
    REQUIRE(grading_from_score(101) == GRADE_NOT_ENTERED);
    REQUIRE(grading_from_score(NAN) == GRADE_NOT_ENTERED);
    REQUIRE(grading_from_score(49.99) == 0);
    REQUIRE(grading_from_score(50.0) == 1);
    REQUIRE(!grading_calculate(&student, &summary));
    REQUIRE(!student_create(&student, "Alice", 0));
    REQUIRE(!student_create(&student, "Alice", MAX_SUBJECT_COUNT + 1));

    for (int repetition = 0; repetition < 100; repetition++) {
        REQUIRE(student_create(&student, "Alice", MAX_SUBJECT_COUNT));
        for (size_t index = 0; index < student.subject_count; index++) {
            REQUIRE(student.subjects[index].name[0] == '\0');
            REQUIRE(student.subjects[index].score == GRADE_NOT_ENTERED);
            REQUIRE(student.subjects[index].grade == GRADE_NOT_ENTERED);
        }
        REQUIRE(!student_create(&student, "Bob", 1));
        student_destroy(&student);
        student_destroy(&student);
        REQUIRE(student.subjects == NULL && student.subject_count == 0);
    }
    puts("PASS: allocation, initialization, repeated cleanup and invalid parameters");

    REQUIRE(student_create(&student, "Alice", 1));
    REQUIRE(!grading_calculate(&student, &summary));
    strcpy(student.subjects[0].name, "Math");
    student.subjects[0].score = 85;
    REQUIRE(grading_calculate(&student, &summary));
    REQUIRE(summary.average_grade == 4 && summary.average_score == 85);
    /* Printing to a readable but non-writable stream must fail. */
    read_only = fopen("CMakeLists.txt", "r");
    REQUIRE(read_only != NULL);
    REQUIRE(!report_print(read_only, &student, &summary));
    fclose(read_only);
    student_destroy(&student);
    puts("PASS: incomplete data rejected, calculations and output write errors checked");
    return EXIT_SUCCESS;
}
