#!/bin/sh
# Runs a scripted session (samples/demo_input.txt) with a fixed seed and checks the output.
# Usage: sh tests/integration.sh <program>
APP="${1:-bin/iot_monitor}"
OUT="bin/integration_output.txt"

"$APP" --seed 42 < samples/demo_input.txt > "$OUT" 2> bin/integration_stderr.txt
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
expect_at_least() { # text, minimum number of lines that contain it
    found=$(grep -cF -- "$1" "$OUT")
    if [ "$found" -ge "$2" ]; then
        echo "ok   $1 (x$found)"
    else
        echo "FAIL $1: found $found, wanted at least $2"
        failures=$((failures + 1))
    fi
}

expect "Project: Dynamic IoT Event Monitoring and Callback Engine"
expect "Registered 6 starting devices."
expect "Generated 12 readings. 12 waiting to be processed."
expect "Processed 12 readings."

# Every type is dispatched to its own callback, with normal and abnormal results.
expect_at_least " temperature_monitor " 4
expect_at_least " pressure_monitor " 4
expect_at_least " battery_monitor " 4
expect_at_least "NORMAL (range" 10
expect_at_least "ALERT:" 3
expect_at_least "DRIFT:" 1

# Registration and removal.
expect "Device 401 registered. Total devices: 7"
expect "Rejected: a device with ID 401 already exists."
expect "Invalid input. Enter a whole number from 1 to 99999."
expect "Device 402 registered. Total devices: 8"
expect "Device 202 removed. Total devices: 7"
expect "No device with ID 999, nothing removed."
expect "reading for device 202 skipped: device was removed"

# Alert report and summary.
expect "Expected: 15.00 to 45.00 C"
expect "Callback: drift_detector"
expect "Readings processed: 26"
expect "Readings skipped (device removed): 1"
expect "Out-of-range alerts per device type:"
expect "Custom callback - Sensor Drift Detector:"
expect "All memory released. Goodbye."

if [ "$failures" -ne 0 ]; then
    echo "$failures integration check(s) failed. Output is in $OUT"
    exit 1
fi
echo "integration: all checks passed"
