#ifndef DISPLAY_H
#define DISPLAY_H

#include "student.h"

// Prints the column headings of the student table.
void print_table_header(void);

// Prints one student as a table row.
void print_student_row(const Student *student);

// Prints every student as a formatted table, followed by the record count.
void print_student_table(const StudentList *list);

#endif
