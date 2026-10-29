#ifndef SORT_H
#define SORT_H

#include "student.h"

// A comparison strategy for qsort; pick one to choose how students are ordered.
typedef int (*StudentCompare)(const void *a, const void *b);

int compare_by_grade_desc(const void *a, const void *b); // highest grade first
int compare_by_name(const void *a, const void *b);       // A to Z, ignoring case
int compare_by_id(const void *a, const void *b);         // smallest ID first

// Sorts the list in place using the given strategy.
void sort_students_by(StudentList *list, StudentCompare compare);

#endif
