#include "dataset.h"
#include "log.h"
#include "records.h"

static const Student INITIAL_STUDENTS[] = {
    {1001, "Chico Mugisha", "Embedded C", 78.5f},
    {1002, "Gisele Uwase", "Embedded C", 91.0f},
    {1003, "Soleil Ishimwe", "Embedded C", 46.5f},
    {1004, "Elina Uwimana", "Data Structures", 83.0f},
    {1005, "Queen Ingabire", "Data Structures", 67.5f},
    {1006, "Derick Habimana", "Data Structures", 38.0f},
    {1007, "Nina Mutoni", "Linear Algebra", 72.0f},
    {1008, "Bonheur Niyonzima", "Linear Algebra", 55.5f},
    {1009, "Consolee Mukamana", "Linear Algebra", 88.5f},
    {1010, "Manzi Byiringiro", "Web Design", 49.0f},
    {1011, "Belyse Iradukunda", "Web Design", 94.5f},
    {1012, "Muzungu Gatete", "Web Design", 61.0f},
    {1013, "Geovanie Nshimiyimana", "Embedded C", 58.0f},
    {1014, "Shukurani Hakizimana", "Data Structures", 75.5f},
    {1015, "Allen Tuyishime", "Linear Algebra", 42.5f},
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
