#include "analysis.h"
#include "records.h"
#include "sort.h"
#include "stats.h"
#include "test.h"

#include <stdio.h>
#include <time.h>

#define RECORDS 10000 // each add scans for duplicate IDs, so n adds cost O(n^2)
#define RISK_RECORDS 2000
#define BUDGET_SECONDS 1.0

static double seconds_since(clock_t start)
{
    return (double)(clock() - start) / CLOCKS_PER_SEC;
}

// Times one step and fails the check if it goes over the budget.
#define TIMED(label, code)                                              \
    do {                                                                \
        clock_t start_ = clock();                                       \
        code;                                                           \
        double took_ = seconds_since(start_);                           \
        printf("%-28s %.3fs\n", label, took_);                          \
        CHECK(took_ < BUDGET_SECONDS);                                  \
    } while (0)

static Student make(int id)
{
    Student s = {0};
    s.id = id;
    snprintf(s.name, sizeof s.name, "Student %d", (id * 7919) % RECORDS);
    snprintf(s.course, sizeof s.course, "Course %d", id % 8);
    s.grade = (float)((id * 37) % 101);
    return s;
}

// Large-input timing: adds, sorts, stats, search, removal and the risk analysis.
int main(void)
{
    StudentList list;
    CHECK(list_init(&list, 0));

    TIMED("add 10k (growing array)", for (int id = 1; id <= RECORDS; id++) {
        Student s = make(id);
        list_add(&list, &s);
    });
    CHECK(list.count == RECORDS);

    TIMED("sort 10k by name", sort_students_by(&list, compare_by_name));
    TIMED("sort 10k by grade", sort_students_by(&list, compare_by_grade_desc));
    TIMED("average + top + lowest", {
        stats_average(&list);
        stats_top(&list);
        stats_lowest(&list);
    });
    TIMED("1k id searches", for (int i = 1; i <= 1000; i++) list_find_id(&list, i * 97));

    StudentList small;
    list_init(&small, 0);
    for (int id = 1; id <= RISK_RECORDS; id++) {
        Student s = make(id);
        list_add(&small, &s);
    }
    TIMED("risk index on 2k", risk_summarise(&small));
    list_free(&small);

    TIMED("remove 1k from the end", for (int id = RECORDS; id > RECORDS - 1000; id--)
                                        list_remove(&list, id));
    CHECK(list.count == RECORDS - 1000);

    list_free(&list);
    return TEST_DONE();
}
