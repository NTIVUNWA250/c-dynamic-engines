#include "dataset.h"
#include "log.h"
#include "records.h"

static const Student INITIAL_STUDENTS[] = {
    {1001, "Aline Mukeshimana", "Embedded C", 84.5f},
    {1002, "David Okello", "Embedded C", 47.0f},
    {1003, "Fatima Diallo", "Embedded C", 91.0f},
    {1004, "Kevin Ndayishimiye", "Data Structures", 58.5f},
    {1005, "Grace Wanjiku", "Data Structures", 73.0f},
    {1006, "Samuel Mensah", "Data Structures", 36.5f},
    {1007, "Nadia Hakizimana", "Linear Algebra", 66.0f},
    {1008, "Patrick Mugisha", "Linear Algebra", 52.0f},
    {1009, "Esther Achieng", "Linear Algebra", 79.5f},
    {1010, "Joseph Tuyisenge", "Web Design", 44.0f},
    {1011, "Mariam Uwimana", "Web Design", 88.0f},
    {1012, "Brian Kamau", "Web Design", 61.5f},
};

#define INITIAL_COUNT (sizeof INITIAL_STUDENTS / sizeof INITIAL_STUDENTS[0])

int dataset_load(StudentList *list)
{
    if (!list_init(list, INITIAL_COUNT))
        return 0;
    for (const Student *s = INITIAL_STUDENTS; s < INITIAL_STUDENTS + INITIAL_COUNT; s++) {
        if (list_add(list, s) != REC_OK) {
            log_msg(LOG_ERROR, "dataset", "could not load student %d", s->id);
            list_free(list);
            return 0;
        }
    }
    log_msg(LOG_INFO, "dataset", "loaded %zu students", list->count);
    return 1;
}
