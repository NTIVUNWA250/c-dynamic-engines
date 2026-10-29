#ifndef REPORT_H
#define REPORT_H

#include "student.h"

// Prints each student's Academic Risk Index and the class-wide results.
void print_risk_report(const StudentList *list);

// Prints the summary: totals, average, extremes, pass counts and risk results.
void print_summary_report(const StudentList *list);

#endif
