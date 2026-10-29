#ifndef SORT_H
#define SORT_H

#include "student.h"

typedef int (*StudentCompare)(const void *a, const void *b);

int compare_by_grade_desc(const void *a, const void *b);
int compare_by_name(const void *a, const void *b);
int compare_by_id(const void *a, const void *b);

void sort_students_by(StudentList *list, StudentCompare compare);

#endif
