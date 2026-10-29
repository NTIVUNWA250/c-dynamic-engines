#ifndef MONITOR_H
#define MONITOR_H

#include "device.h"

#define REASON_LEN 112
#define NOTE_LEN 128

typedef struct Alert {
    int device_id;
    char device_name[DEVICE_NAME_LEN];
    DeviceType type;
    double value;
    double expected_low, expected_high;
    char reason[REASON_LEN];
    const char *callback;
} Alert;

typedef struct Monitor {
    Alert *alerts;
    size_t alert_count;
    size_t alert_capacity;

    size_t readings_processed;
    size_t normal_readings;
    size_t abnormal_readings;
    size_t range_alerts_per_type[DEVICE_TYPE_COUNT];

    size_t drift_checks;
    size_t drift_events;
    size_t drift_per_type[DEVICE_TYPE_COUNT];
    double largest_drift_ratio;
    int largest_drift_device;
    double largest_drift_change;

    char last_note[NOTE_LEN];
    char drift_note[NOTE_LEN];
} Monitor;

int monitor_init(Monitor *monitor);
void monitor_free(Monitor *monitor);

void monitor_bind(Monitor *monitor);
Monitor *monitor_active(void);

int monitor_add_alert(Monitor *monitor, const Device *device, double low, double high,
                      const char *reason, const char *callback);

#endif
