#ifndef RECORDS_H
#define RECORDS_H

#include "student.h"

// Why an add was accepted or rejected.
typedef enum {
    REC_OK,
    REC_DUPLICATE_ID,
    REC_INVALID_ID,
    REC_INVALID_GRADE,
    REC_EMPTY_TEXT,
    REC_NO_MEMORY
} RecordResult;

// Allocates room for initial_capacity students with calloc. Returns 1 on success.
int list_init(StudentList *list, size_t initial_capacity);

// Frees the array and resets the list to empty.
void list_free(StudentList *list);

// Validates and appends a copy of student, growing the array with realloc if full.
RecordResult list_add(StudentList *list, const Student *student);

// Removes the student with this id and shrinks the array when it is mostly empty.
// Returns 1 if a record was removed, 0 if the id was not found.
int list_remove(StudentList *list, int id);

// Returns the student with this id, or NULL.
Student *list_find_id(const StudentList *list, int id);

// Returns the next student after `after` (NULL = from the start) whose full name,
// or any single word of it, matches name ignoring case. NULL when there are no more.
const Student *list_find_name(const StudentList *list, const char *name, const Student *after);

// Human-readable text for a RecordResult.
const char *record_result_text(RecordResult result);

#endif
