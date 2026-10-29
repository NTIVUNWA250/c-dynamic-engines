#include "stats.h"

float stats_average(const StudentList *list)
{
    if (list->count == 0)
        return 0.0f;
    double total = 0.0;
    for (const Student *p = list->items, *end = p + list->count; p < end; p++)
        total += p->grade;
    return (float)(total / (double)list->count);
}

const Student *stats_top(const StudentList *list)
{
    const Student *best = NULL;
    for (const Student *p = list->items, *end = p + list->count; p < end; p++)
        if (best == NULL || p->grade > best->grade)
            best = p;
    return best;
}

const Student *stats_lowest(const StudentList *list)
{
    const Student *worst = NULL;
    for (const Student *p = list->items, *end = p + list->count; p < end; p++)
        if (worst == NULL || p->grade < worst->grade)
            worst = p;
    return worst;
}

size_t stats_count_passing(const StudentList *list, float pass_mark)
{
    size_t passing = 0;
    for (const Student *p = list->items, *end = p + list->count; p < end; p++)
        if (p->grade >= pass_mark)
            passing++;
    return passing;
}
