#include "actions.h"
#include "display.h"
#include "input.h"
#include "log.h"
#include "records.h"
#include "report.h"
#include "sort.h"
#include "stats.h"

#include <stdio.h>

void add_student(StudentList *list)
{
    Student s = {0};
    printf("\n-- Add a student --\n");
    if (!read_int("Student ID (1-999999): ", ID_MIN, ID_MAX, &s.id))
        return;
    // Reject a duplicate straight away instead of after asking for everything else.
    if (list_find_id(list, s.id) != NULL) {
        printf("Rejected: a student with ID %d already exists.\n", s.id);
        return;
    }
    if (!read_text("Full name: ", s.name, sizeof s.name) ||
        !read_text("Course: ", s.course, sizeof s.course) ||
        !read_float("Grade (0-100): ", GRADE_MIN, GRADE_MAX, &s.grade))
        return;

    RecordResult result = list_add(list, &s);
    if (result == REC_OK)
        printf("Student %d added. Total records: %zu\n", s.id, list->count);
    else
        printf("Rejected: %s.\n", record_result_text(result));
}

void display_students(StudentList *list)
{
    printf("\n-- All students --\n");
    print_student_table(list);
}

// The sort choices, each paired with its comparison function.
static const struct {
    const char *label;
    StudentCompare compare;
} SORT_OPTIONS[] = {
    {"Grade (highest first)", compare_by_grade_desc},
    {"Name (A to Z)", compare_by_name},
    {"Student ID (smallest first)", compare_by_id},
};

void sort_students(StudentList *list)
{
    int count = (int)(sizeof SORT_OPTIONS / sizeof SORT_OPTIONS[0]);
    int choice;
    printf("\n-- Sort students --\n");
    for (int i = 0; i < count; i++)
        printf("%d. %s\n", i + 1, SORT_OPTIONS[i].label);
    if (!read_int("Sort by: ", 1, count, &choice))
        return;

    sort_students_by(list, SORT_OPTIONS[choice - 1].compare);
    printf("Sorted by %s.\n", SORT_OPTIONS[choice - 1].label);
    print_student_table(list);
}

void calculate_average(StudentList *list)
{
    if (list->count == 0) {
        printf("No records, so there is no average.\n");
        return;
    }
    printf("Average grade of %zu students: %.2f\n", list->count, stats_average(list));
}

void find_top_student(StudentList *list)
{
    const Student *top = stats_top(list);
    if (top == NULL) {
        printf("No records, so there is no top student.\n");
        return;
    }
    printf("\nTop-performing student:\n");
    print_table_header();
    print_student_row(top);
}

// Prints every student whose name or one name word matches the query.
static void search_by_name(const StudentList *list)
{
    char query[NAME_LEN];
    if (!read_text("Name, first name or surname: ", query, sizeof query))
        return;
    size_t matches = 0;
    for (const Student *s = list_find_name(list, query, NULL); s != NULL;
         s = list_find_name(list, query, s)) {
        if (matches++ == 0)
            print_table_header();
        print_student_row(s);
    }
    if (matches == 0)
        printf("No student named \"%s\".\n", query);
    else
        printf("%zu match(es).\n", matches);
}

void search_student(StudentList *list)
{
    int mode;
    printf("\n-- Search --\n1. By student ID\n2. By name\n");
    if (!read_int("Search by: ", 1, 2, &mode))
        return;
    if (mode == 2) {
        search_by_name(list);
        return;
    }
    int id;
    if (!read_int("Student ID: ", ID_MIN, ID_MAX, &id))
        return;
    const Student *found = list_find_id(list, id);
    if (found == NULL) {
        printf("No student with ID %d.\n", id);
        return;
    }
    print_table_header();
    print_student_row(found);
}

void remove_student(StudentList *list)
{
    int id;
    printf("\n-- Remove a student --\n");
    if (!read_int("Student ID to remove: ", ID_MIN, ID_MAX, &id))
        return;
    if (list_remove(list, id))
        printf("Student %d removed. Total records: %zu\n", id, list->count);
    else
        printf("No student with ID %d, nothing removed.\n", id);
}

void custom_analysis(StudentList *list)
{
    print_risk_report(list);
}

void generate_summary_report(StudentList *list)
{
    print_summary_report(list);
}
