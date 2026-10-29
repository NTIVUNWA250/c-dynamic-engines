#ifndef MONITOR_H
#define MONITOR_H

#include "device.h"

#define REASON_LEN 112
#define NOTE_LEN 128

// One abnormal reading, kept for the alert report.
typedef struct Alert {
    int device_id;
    char device_name[DEVICE_NAME_LEN];
    DeviceType type;
    double value;
    double expected_low, expected_high;
    char reason[REASON_LEN];
    const char *callback; // name of the callback that raised it
} Alert;

// Everything the callbacks record: a growable alert log and the running counters.
typedef struct Monitor {
    Alert *alerts;
    size_t alert_count;
    size_t alert_capacity;

    size_t readings_processed;
    size_t normal_readings;
    size_t abnormal_readings;
    size_t range_alerts_per_type[DEVICE_TYPE_COUNT];

    // Results of the custom drift_detector callback.
    size_t drift_checks;
    size_t drift_events;
    size_t drift_per_type[DEVICE_TYPE_COUNT];
    double largest_drift_ratio; // biggest change divided by its type's drift limit
    int largest_drift_device;   // device id, 0 if none
    double largest_drift_change;

    char last_note[NOTE_LEN];  // result of the last range check
    char drift_note[NOTE_LEN]; // result of the last drift check (empty if fine)
} Monitor;

// Starts with an empty alert log. Returns 1 on success.
int monitor_init(Monitor *monitor);
void monitor_free(Monitor *monitor);

// Callbacks only receive a Device, so they record results into the bound monitor.
void monitor_bind(Monitor *monitor);
Monitor *monitor_active(void);

// Appends an alert, growing the log with realloc. Returns 0 if memory runs out.
int monitor_add_alert(Monitor *monitor, const Device *device, double low, double high,
                      const char *reason, const char *callback);

#endif
