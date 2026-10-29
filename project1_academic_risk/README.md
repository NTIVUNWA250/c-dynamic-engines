# Project 1: Dynamic Academic Performance and Risk Analysis Engine

A menu-driven C program that manages student records in a growable heap array,
sorts and searches them, and scores each student with an **Academic Risk Index**.

## Build and run

```
make                         # builds bin/academic_engine
./bin/academic_engine
```

Make a sample output from the scripted session:

```
./bin/academic_engine < samples/demo_input.txt > samples/sample_output.txt
```

## Test

```
make test        # unit + integration + performance
make unit        # unit tests only (AddressSanitizer + UBSan)
```

## Menu

| # | Operation | Function |
|---|---|---|
| 1 | Add a new student | `add_student()` |
| 2 | Display all students | `display_students()` |
| 3 | Sort by grade, name or ID | `sort_students()` |
| 4 | Average grade | `calculate_average()` |
| 5 | Top-performing student | `find_top_student()` |
| 6 | Search by ID or name | `search_student()` |
| 7 | Remove a student | `remove_student()` |
| 8 | Academic Risk Index | `custom_analysis()` |
| 9 | Summary report | `generate_summary_report()` |
| 10 | Exit | frees memory and quits |

## Where each requirement lives

| Requirement | File |
|---|---|
| `struct Student` (id, name, course, grade) | `include/student.h` |
| Starting dataset of 12 students, header with student ID | `src/dataset.c`, `src/main.c`, `include/config.h` |
| Function-pointer menu table (`MenuAction`) | `src/menu.c`, `include/actions.h` |
| Function-pointer sort strategies (`StudentCompare` + `qsort`) | `src/sort.c`, `src/actions.c` |
| `calloc` for the starting array, `realloc` to grow and shrink, `free` on exit | `src/records.c` |
| Allocation results checked; failure leaves the list unchanged | `src/records.c`, `tests/test_alloc_failure.c` |
| Pointer traversal (`for (p = items; p < end; p++)`) | `src/stats.c`, `src/display.c`, `src/records.c` |
| `strcmp` for name search and name sort | `src/records.c`, `src/text_util.c` |
| Safe string input (no overflow; long input is cut) | `../common/input.c` |
| Duplicate IDs, bad grades and bad menu input rejected | `src/records.c`, `../common/input.c` |
| Custom analysis | `src/analysis.c`, `docs/custom_analysis.md` |
| Summary report | `src/report.c` |

## Memory growth and shrinking

- The starting array is `calloc`ed for exactly 12 records.
- When it is full, `realloc` doubles the capacity.
- When three quarters of it is unused after a removal, `realloc` halves it.
- Removing the last record frees the array.
- The display shows `Records: N (memory reserved for M)`, so you can see this happen.

## Name search

Names are matched ignoring case, either as the full name or as any single word of it.
For example, `grace` finds "Grace Wanjiku". Each comparison uses `strcmp` on
lowercase copies.

## Custom analysis

See `docs/custom_analysis.md`.
