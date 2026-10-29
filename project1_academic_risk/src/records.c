#include "records.h"
#include "log.h"
#include "text_util.h"

#include <stdlib.h>
#include <string.h>

#define MIN_CAPACITY 4

int list_init(StudentList *list, size_t initial_capacity)
{
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
    if (initial_capacity == 0)
        return 1;

    list->items = calloc(initial_capacity, sizeof *list->items);
    if (list->items == NULL) {
        log_msg(LOG_ERROR, "records", "could not allocate %zu records", initial_capacity);
        return 0;
    }
    list->capacity = initial_capacity;
    log_msg(LOG_INFO, "records", "allocated %zu records at %p", initial_capacity,
            (void *)list->items);
    return 1;
}

void list_free(StudentList *list)
{
    free(list->items);
    log_msg(LOG_INFO, "records", "freed %zu records", list->capacity);
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

static int resize(StudentList *list, size_t new_capacity)
{
    Student *grown = realloc(list->items, new_capacity * sizeof *grown);
    if (grown == NULL) {
        log_msg(LOG_ERROR, "records", "realloc to %zu records failed", new_capacity);
        return 0;
    }
    log_msg(LOG_INFO, "records", "capacity %zu -> %zu", list->capacity, new_capacity);
    list->items = grown;
    list->capacity = new_capacity;
    return 1;
}

static RecordResult validate(const StudentList *list, const Student *s)
{
    if (s->id < ID_MIN || s->id > ID_MAX)
        return REC_INVALID_ID;
    if (!(s->grade >= GRADE_MIN && s->grade <= GRADE_MAX))
        return REC_INVALID_GRADE;
    if (s->name[0] == '\0' || s->course[0] == '\0')
        return REC_EMPTY_TEXT;
    if (list_find_id(list, s->id) != NULL)
        return REC_DUPLICATE_ID;
    return REC_OK;
}

RecordResult list_add(StudentList *list, const Student *student)
{
    RecordResult result = validate(list, student);
    if (result != REC_OK) {
        log_msg(LOG_INFO, "records", "rejected id %d: %s", student->id,
                record_result_text(result));
        return result;
    }

    if (list->count == list->capacity) {
        size_t new_capacity = list->capacity < MIN_CAPACITY ? MIN_CAPACITY : list->capacity * 2;
        if (!resize(list, new_capacity))
            return REC_NO_MEMORY;
    }

    Student *slot = list->items + list->count;
    *slot = *student;
    slot->name[NAME_LEN - 1] = '\0';
    slot->course[COURSE_LEN - 1] = '\0';
    list->count++;
    log_msg(LOG_INFO, "records", "added id %d (%zu records)", slot->id, list->count);
    return REC_OK;
}

int list_remove(StudentList *list, int id)
{
    Student *target = list_find_id(list, id);
    if (target == NULL)
        return 0;

    Student *end = list->items + list->count;
    memmove(target, target + 1, (size_t)(end - target - 1) * sizeof *target);
    list->count--;
    log_msg(LOG_INFO, "records", "removed id %d (%zu records)", id, list->count);

    if (list->count == 0) {
        free(list->items);
        list->items = NULL;
        list->capacity = 0;
        log_msg(LOG_INFO, "records", "list empty, memory released");
    } else if (list->capacity > MIN_CAPACITY && list->count <= list->capacity / 4) {
        resize(list, list->capacity / 2);
    }
    return 1;
}

Student *list_find_id(const StudentList *list, int id)
{
    for (Student *p = list->items, *end = list->items + list->count; p < end; p++)
        if (p->id == id)
            return p;
    return NULL;
}

static int name_matches(const char *name, const char *query)
{
    char lowered_name[NAME_LEN];
    char lowered_query[NAME_LEN];
    to_lower_copy(lowered_name, name, sizeof lowered_name);
    to_lower_copy(lowered_query, query, sizeof lowered_query);

    if (strcmp(lowered_name, lowered_query) == 0)
        return 1;

    char *word = lowered_name;
    while (*word != '\0') {
        char *space = strchr(word, ' ');
        if (space != NULL)
            *space = '\0';
        if (strcmp(word, lowered_query) == 0)
            return 1;
        if (space == NULL)
            break;
        word = space + 1;
    }
    return 0;
}

const Student *list_find_name(const StudentList *list, const char *name, const Student *after)
{
    const Student *p = after == NULL ? list->items : after + 1;
    const Student *end = list->items + list->count;
    for (; p < end; p++)
        if (name_matches(p->name, name))
            return p;
    return NULL;
}

const char *record_result_text(RecordResult result)
{
    switch (result) {
    case REC_OK:
        return "record added";
    case REC_DUPLICATE_ID:
        return "a student with this ID already exists";
    case REC_INVALID_ID:
        return "ID must be between 1 and 999999";
    case REC_INVALID_GRADE:
        return "grade must be between 0 and 100";
    case REC_EMPTY_TEXT:
        return "name and course cannot be empty";
    case REC_NO_MEMORY:
        return "not enough memory to add the record";
    }
    return "unknown result";
}
