#include "sort.h"
#include "log.h"
#include "text_util.h"

#include <stdlib.h>

// Ties are broken by ID so every sort gives the same order each time.
static int by_id(const Student *a, const Student *b)
{
    return (a->id > b->id) - (a->id < b->id);
}

int compare_by_grade_desc(const void *a, const void *b)
{
    const Student *x = a;
    const Student *y = b;
    if (x->grade != y->grade)
        return x->grade < y->grade ? 1 : -1;
    return by_id(x, y);
}

int compare_by_name(const void *a, const void *b)
{
    const Student *x = a;
    const Student *y = b;
    int order = compare_ignore_case(x->name, y->name);
    return order != 0 ? order : by_id(x, y);
}

int compare_by_id(const void *a, const void *b)
{
    return by_id(a, b);
}

void sort_students_by(StudentList *list, StudentCompare compare)
{
    if (list->count < 2)
        return;
    qsort(list->items, list->count, sizeof *list->items, compare);
    log_msg(LOG_INFO, "sort", "sorted %zu records", list->count);
}
