#include "callbacks.h"
#include "monitor.h"
#include "registry.h"
#include "test.h"

#include <string.h>

// Range callbacks per type, dispatch by table, and the drift detector.
int main(void)
{
    Monitor m;
    monitor_init(&m);
    monitor_bind(&m);

    DeviceList list;
    devices_init(&list, 4);
    devices_register(&list, 1, "Oven", DEVICE_TEMPERATURE);
    devices_register(&list, 2, "Pipe", DEVICE_PRESSURE);
    devices_register(&list, 3, "Cell", DEVICE_BATTERY);
    Device *t = devices_find(&list, 1);
    Device *p = devices_find(&list, 2);
    Device *b = devices_find(&list, 3);

    // The table hands each type its own callback.
    CHECK(callback_for_type(DEVICE_TEMPERATURE) == temperature_monitor);
    CHECK(callback_for_type(DEVICE_PRESSURE) == pressure_monitor);
    CHECK(callback_for_type(DEVICE_BATTERY) == battery_monitor);
    CHECK(callback_for_type(DEVICE_TYPE_COUNT) == NULL);
    CHECK(strcmp(callback_name_for_type(DEVICE_BATTERY), "battery_monitor") == 0);

    // Normal readings, including the exact edges of each range.
    t->readings.temperature = 45.0f;
    process_device(t, callback_for_type(t->type));
    p->readings.pressure = 950;
    process_device(p, callback_for_type(p->type));
    b->readings.voltage = 3.70f;
    process_device(b, callback_for_type(b->type));
    CHECK(t->status == STATUS_NORMAL && p->status == STATUS_NORMAL && b->status == STATUS_NORMAL);
    CHECK(m.readings_processed == 3 && m.normal_readings == 3 && m.alert_count == 0);

    // Abnormal readings: one per type, each with its own reason and callback name.
    t->readings.temperature = 52.5f;
    process_device(t, callback_for_type(t->type));
    p->readings.pressure = 900;
    process_device(p, callback_for_type(p->type));
    b->readings.voltage = 3.10f;
    process_device(b, callback_for_type(b->type));
    CHECK(t->status == STATUS_ALERT && p->status == STATUS_ALERT && b->status == STATUS_ALERT);
    CHECK(m.abnormal_readings == 3 && m.alert_count == 3);
    CHECK(strstr(m.alerts[0].reason, "overheating") != NULL);
    CHECK(strcmp(m.alerts[0].callback, "temperature_monitor") == 0);
    CHECK(m.alerts[0].expected_low == 15.0 && m.alerts[0].expected_high == 45.0);
    CHECK(strstr(m.alerts[1].reason, "under-pressure") != NULL);
    CHECK(m.alerts[1].value == 900.0);
    CHECK(strstr(m.alerts[2].reason, "low voltage") != NULL);
    CHECK(m.range_alerts_per_type[DEVICE_TEMPERATURE] == 1);
    CHECK(m.range_alerts_per_type[DEVICE_PRESSURE] == 1);
    CHECK(m.range_alerts_per_type[DEVICE_BATTERY] == 1);

    // A NULL callback or device is ignored safely.
    process_device(t, NULL);
    process_device(NULL, temperature_monitor);
    CHECK(m.readings_processed == 6);

    // Drift: first reading sets the baseline, small change is fine, big jump alerts.
    Device *d = t;
    d->has_previous = 0;
    d->readings.temperature = 20.0f;
    process_device(d, drift_detector);
    CHECK(m.drift_events == 0 && d->has_previous && d->previous == 20.0);
    d->readings.temperature = 29.0f; // +9, limit is 10
    process_device(d, drift_detector);
    CHECK(m.drift_events == 0 && m.drift_note[0] == '\0');
    d->readings.temperature = 41.5f; // +12.5 > 10
    d->status = STATUS_NORMAL;
    process_device(d, drift_detector);
    CHECK(m.drift_events == 1 && d->status == STATUS_ALERT);
    CHECK(m.drift_per_type[DEVICE_TEMPERATURE] == 1);
    Alert *last = &m.alerts[m.alert_count - 1];
    CHECK(strcmp(last->callback, "drift_detector") == 0);
    CHECK(strstr(last->reason, "sudden rise of 12.50 C") != NULL);
    CHECK(last->expected_low == 19.0 && last->expected_high == 39.0); // previous 29 +/- 10
    CHECK(m.largest_drift_device == 1);
    CHECK_NEAR(m.largest_drift_change, 12.5, 1e-6);

    // Drift on an integer pressure reading uses the pressure member and limit.
    p->has_previous = 0;
    p->readings.pressure = 1000;
    process_device(p, drift_detector);
    p->readings.pressure = 930; // -70 > 60
    process_device(p, drift_detector);
    CHECK(m.drift_per_type[DEVICE_PRESSURE] == 1);
    CHECK(strstr(m.alerts[m.alert_count - 1].reason, "sudden drop of 70.00 hPa") != NULL);
    CHECK(m.drift_checks == 5);

    devices_free(&list);
    monitor_free(&m);
    CHECK(monitor_active() == NULL);
    return TEST_DONE();
}
