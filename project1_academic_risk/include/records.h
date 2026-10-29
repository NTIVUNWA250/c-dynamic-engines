#ifndef RECORDS_H
#define RECORDS_H

#include "student.h"

typedef enum {
    REC_OK,
    REC_DUPLICATE_ID,
    REC_INVALID_ID,
    REC_INVALID_GRADE,
    REC_EMPTY_TEXT,
    REC_NO_MEMORY
} RecordResult;

int list_init(StudentList *list, size_t initial_capacity);

void list_free(StudentList *list);

RecordResult list_add(StudentList *list, const Student *student);

int list_remove(StudentList *list, int id);

Student *list_find_id(const StudentList *list, int id);

const Student *list_find_name(const StudentList *list, const char *name, const Student *after);

const char *record_result_text(RecordResult result);

#endif
