#!/bin/sh
APP="${1:-bin/academic_engine}"
OUT="bin/integration_output.txt"

"$APP" < samples/demo_input.txt > "$OUT" 2> bin/integration_stderr.txt
status=$?
if [ "$status" -ne 0 ]; then
    echo "FAIL: program exited with status $status"
    cat bin/integration_stderr.txt
    exit 1
fi

failures=0
expect() {
    if grep -qF -- "$1" "$OUT"; then
        echo "ok   $1"
    else
        echo "FAIL missing: $1"
        failures=$((failures + 1))
    fi
}

expect "Project: Dynamic Academic Performance and Risk Analysis Engine"
expect "Student ID: NTIVUNWA250"
expect "Loaded 15 starting student records."
expect "Records: 15 (memory reserved for 15)"
expect "Invalid input. Enter a whole number from 1 to 10."
expect "Invalid input. Enter a number from 0.0 to 100.0."
expect "Student 1016 added. Total records: 16"
expect "Rejected: a student with ID 1016 already exists."
expect "Student 1023 added. Total records: 23"
expect "Records: 23 (memory reserved for 30)"
expect "Sorted by Grade (highest first)."
expect "Sorted by Name (A to Z)."
expect "Average grade of 23 students: 67.67"
expect "Top-performing student:"
expect "Christella Uwera"
expect "Gisele Uwase"
expect "No student named \"Kevin\"."
expect "Student 1020 removed. Total records: 22"
expect "Student 1006 removed. Total records: 21"
expect "No student with ID 9999, nothing removed."
expect "Records: 21 (memory reserved for 30)"
expect "Academic Risk Index (0 = no risk, 100 = highest risk)"
expect "Most at risk: Allen Tuyishime (ID 1015), index 29.4"
expect "Total student records: 21"
expect "Highest grade: 94.5 (Belyse Iradukunda, ID 1011)"
expect "Students passing (>= 50): 18"
expect "Students below pass mark: 3"
expect "All memory released. Goodbye."

if [ "$failures" -ne 0 ]; then
    echo "$failures integration check(s) failed. Output is in $OUT"
    exit 1
fi
echo "integration: all checks passed"
