#ifndef ACTIONS_H
#define ACTIONS_H

#include "student.h"

typedef void (*MenuAction)(StudentList *list);

void add_student(StudentList *list);
void display_students(StudentList *list);
void sort_students(StudentList *list);
void calculate_average(StudentList *list);
void find_top_student(StudentList *list);
void search_student(StudentList *list);
void remove_student(StudentList *list);
void custom_analysis(StudentList *list);
void generate_summary_report(StudentList *list);

#endif
