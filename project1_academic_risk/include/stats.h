#ifndef STATS_H
#define STATS_H

#include "student.h"

#define PASS_MARK 50.0f

float stats_average(const StudentList *list);

const Student *stats_top(const StudentList *list);
const Student *stats_lowest(const StudentList *list);

size_t stats_count_passing(const StudentList *list, float pass_mark);

#endif
