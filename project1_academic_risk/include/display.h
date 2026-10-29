#ifndef DISPLAY_H
#define DISPLAY_H

#include "student.h"

void print_table_header(void);

void print_student_row(const Student *student);

void print_student_table(const StudentList *list);

#endif
