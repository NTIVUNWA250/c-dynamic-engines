#include "analysis.h"
#include "records.h"
#include "test.h"

#include <string.h>

static void add(StudentList *list, int id, const char *course, float grade)
{
    Student s = {0};
    s.id = id;
    strcpy(s.name, "Student");
    strncpy(s.course, course, COURSE_LEN - 1);
    s.grade = grade;
    list_add(list, &s);
}

// Formula values, band edges, course averages and the class summary.
int main(void)
{
    // Worked examples from docs/custom_analysis.md.
    CHECK_NEAR(risk_index(90.0f, 70.0f), 0.0, 0.001);
    CHECK_NEAR(risk_index(75.0f, 80.0f), 1.5, 0.001);
    CHECK_NEAR(risk_index(55.0f, 60.0f), 70.0 * 5.0 / 60.0 + 1.5, 0.001);
    CHECK_NEAR(risk_index(30.0f, 70.0f), 35.0 + 12.0, 0.001);
    CHECK_NEAR(risk_index(0.0f, 100.0f), 100.0, 0.001); // the maximum possible score

    CHECK(risk_level(0.0f) == RISK_NONE);
    CHECK(risk_level(9.99f) == RISK_LOW);
    CHECK(risk_level(10.0f) == RISK_MODERATE);
    CHECK(risk_level(24.99f) == RISK_MODERATE);
    CHECK(risk_level(25.0f) == RISK_HIGH);
    CHECK(strcmp(risk_level_name(RISK_HIGH), "High") == 0);

    // A failing grade is never rated None or Low.
    CHECK(risk_level(risk_index(49.0f, 49.0f)) >= RISK_MODERATE);

    StudentList list;
    list_init(&list, 4);
    RiskSummary empty = risk_summarise(&list);
    CHECK(empty.most_at_risk == NULL && empty.mean_index == 0.0f);

    add(&list, 1, "Maths", 90.0f);
    add(&list, 2, "Maths", 30.0f);
    add(&list, 3, "Art", 58.0f);
    add(&list, 4, "Art", 62.0f);

    CHECK_NEAR(course_average(&list, "Maths"), 60.0, 0.001);
    CHECK_NEAR(course_average(&list, "Art"), 60.0, 0.001);
    CHECK(course_average(&list, "History") == 0.0f);

    RiskSummary summary = risk_summarise(&list);
    CHECK(summary.most_at_risk != NULL && summary.most_at_risk->id == 2);
    CHECK(summary.level_counts[RISK_NONE] == 2);   // 90 and 62
    CHECK(summary.level_counts[RISK_LOW] == 1);    // 58
    CHECK(summary.level_counts[RISK_HIGH] == 1);   // 30
    size_t total = 0;
    for (int i = 0; i < RISK_LEVEL_COUNT; i++)
        total += summary.level_counts[i];
    CHECK(total == list.count);

    list_free(&list);
    return TEST_DONE();
}
