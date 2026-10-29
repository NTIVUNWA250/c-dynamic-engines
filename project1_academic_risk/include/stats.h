#ifndef STATS_H
#define STATS_H

#include "student.h"

#define PASS_MARK 50.0f // a grade at or above this is a pass

// Mean grade, or 0 for an empty list.
float stats_average(const StudentList *list);

// Student with the highest / lowest grade (first one on a tie), or NULL if empty.
const Student *stats_top(const StudentList *list);
const Student *stats_lowest(const StudentList *list);

// Number of students with grade >= pass_mark.
size_t stats_count_passing(const StudentList *list, float pass_mark);

#endif
