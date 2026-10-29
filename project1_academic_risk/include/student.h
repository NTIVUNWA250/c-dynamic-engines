#ifndef STUDENT_H
#define STUDENT_H

#include <stddef.h>

#define NAME_LEN 64
#define COURSE_LEN 48

#define ID_MIN 1
#define ID_MAX 999999
#define GRADE_MIN 0.0f
#define GRADE_MAX 100.0f

typedef struct Student {
    int id;
    char name[NAME_LEN];
    char course[COURSE_LEN];
    float grade;
} Student;

typedef struct StudentList {
    Student *items;
    size_t count;
    size_t capacity;
} StudentList;

#endif
