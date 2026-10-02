#include "grading.h"

enum GradeThreshold {
    SCORE_FOR_GRADE_1 = 50,
    SCORE_FOR_GRADE_2 = 60,
    SCORE_FOR_GRADE_3 = 70,
    SCORE_FOR_GRADE_4 = 80,
    SCORE_FOR_GRADE_5 = 90
};

int grading_from_score(double score)
{
    /* This condition also rejects NaN, whose comparisons are false. */
    if (!(score >= MIN_SCORE && score <= MAX_SCORE)) return GRADE_NOT_ENTERED;
    if (score >= SCORE_FOR_GRADE_5) return 5;
    if (score >= SCORE_FOR_GRADE_4) return 4;
    if (score >= SCORE_FOR_GRADE_3) return 3;
    if (score >= SCORE_FOR_GRADE_2) return 2;
    if (score >= SCORE_FOR_GRADE_1) return 1;
    return 0;
}

bool grading_calculate(Student *student, GradeSummary *summary)
{
    double score_total = 0.0;
    double grade_total = 0.0;

    if (student == NULL || summary == NULL || student->subjects == NULL ||
        student->subject_count < MIN_SUBJECT_COUNT ||
        student->subject_count > MAX_SUBJECT_COUNT) return false;

    /* Validate everything first so incomplete data cannot produce a partial result. */
    for (size_t index = 0; index < student->subject_count; index++) {
        const Subject *subject = &student->subjects[index];
        if (subject->name[0] == '\0' || subject->score < MIN_SCORE ||
            subject->score > MAX_SCORE) return false;
    }

    for (size_t index = 0; index < student->subject_count; index++) {
        Subject *subject = &student->subjects[index];
        subject->grade = grading_from_score(subject->score);
        score_total += subject->score;
        grade_total += subject->grade;
    }

    /* All subjects have equal weight; use floating-point division before formatting. */
    summary->average_grade = grade_total / student->subject_count;
    summary->average_score = score_total / student->subject_count;
    summary->overall_grade = grading_from_score(summary->average_score);
    return true;
}
