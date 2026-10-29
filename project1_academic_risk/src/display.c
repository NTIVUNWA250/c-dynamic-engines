#include "display.h"

#include <stdio.h>

void print_table_header(void)
{
    printf("%-7s %-26s %-22s %6s\n", "ID", "Name", "Course", "Grade");
}

void print_student_row(const Student *student)
{
    printf("%-7d %-26.26s %-22.22s %6.1f\n", student->id, student->name, student->course,
           student->grade);
}

void print_student_table(const StudentList *list)
{
    if (list->count == 0) {
        printf("No student records.\n");
        return;
    }
    print_table_header();
    for (const Student *p = list->items, *end = p + list->count; p < end; p++)
        print_student_row(p);
    printf("Records: %zu (memory reserved for %zu)\n", list->count, list->capacity);
}
