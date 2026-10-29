#!/bin/sh
# Runs a scripted session (samples/demo_input.txt) through the program and checks the output.
# Usage: sh tests/integration.sh <program>
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
expect "Loaded 12 starting student records."
expect "Records: 12"
expect "Invalid input. Enter a whole number from 1 to 10."
expect "Invalid input. Enter a number from 0.0 to 100.0."
expect "Student 1013 added. Total records: 13"
expect "Rejected: a student with ID 1013 already exists."
expect "Student 1014 added. Total records: 14"
expect "Sorted by Grade (highest first)."
expect "Sorted by Name (A to Z)."
expect "Average grade of 14 students:"
expect "Top-performing student:"
expect "Fatima Diallo"
expect "Claire Iradukunda"
expect "No student named \"amina\"."
expect "Grace Wanjiku"
expect "Student 1013 removed. Total records: 13"
expect "No student with ID 9999, nothing removed."
expect "Records: 13"
expect "Academic Risk Index (0 = no risk, 100 = highest risk)"
expect "Most at risk: Olivier Nshimiyimana (ID 1014)"
expect "Total student records: 13"
expect "Students below pass mark:"
expect "All memory released. Goodbye."

if [ "$failures" -ne 0 ]; then
    echo "$failures integration check(s) failed. Output is in $OUT"
    exit 1
fi
echo "integration: all checks passed"
