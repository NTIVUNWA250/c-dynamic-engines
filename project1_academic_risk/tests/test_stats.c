#include "records.h"
#include "sort.h"
#include "stats.h"
#include "test.h"

#include <string.h>

static void add(StudentList *list, int id, const char *name, float grade)
{
    Student s = {0};
    s.id = id;
    strncpy(s.name, name, NAME_LEN - 1);
    strcpy(s.course, "C");
    s.grade = grade;
    list_add(list, &s);
}

int main(void)
{
    StudentList list;
    list_init(&list, 0);

    CHECK(stats_average(&list) == 0.0f);
    CHECK(stats_top(&list) == NULL);
    CHECK(stats_lowest(&list) == NULL);
    CHECK(stats_count_passing(&list, PASS_MARK) == 0);
    sort_students_by(&list, compare_by_name);

    add(&list, 30, "carol", 50.0f);
    add(&list, 10, "Alice", 90.0f);
    add(&list, 20, "Bob", 40.0f);
    add(&list, 40, "dave", 90.0f);

    CHECK_NEAR(stats_average(&list), 67.5, 0.001);
    CHECK(stats_top(&list)->id == 10);
    CHECK(stats_lowest(&list)->id == 20);
    CHECK(stats_count_passing(&list, PASS_MARK) == 3);

    sort_students_by(&list, compare_by_grade_desc);
    CHECK(list.items[0].id == 10 && list.items[1].id == 40);
    CHECK(list.items[2].id == 30 && list.items[3].id == 20);

    sort_students_by(&list, compare_by_name);
    CHECK(strcmp(list.items[0].name, "Alice") == 0);
    CHECK(strcmp(list.items[1].name, "Bob") == 0);
    CHECK(strcmp(list.items[2].name, "carol") == 0);
    CHECK(strcmp(list.items[3].name, "dave") == 0);

    sort_students_by(&list, compare_by_id);
    for (size_t i = 0; i < list.count; i++)
        CHECK(list.items[i].id == (int)(i + 1) * 10);

    list_free(&list);
    return TEST_DONE();
}
