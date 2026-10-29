#include "engine.h"
#include "input.h"

#include <stdio.h>

#define MAX_ROUNDS 50

void generate_readings(Engine *engine)
{
    int rounds;
    if (engine->devices.count == 0) {
        printf("No devices registered, so there is nothing to simulate.\n");
        return;
    }
    printf("Each round gives every device one new reading (%zu per round).\n",
           engine->devices.count);
    if (!read_int("Rounds (1-50): ", 1, MAX_ROUNDS, &rounds))
        return;
    long queued = simulate_rounds(&engine->devices, &engine->queue, rounds);
    if (queued < 0)
        printf("Ran out of memory; %zu readings were queued.\n", engine->queue.count);
    else
        printf("Generated %ld readings. %zu waiting to be processed.\n", queued,
               engine->queue.count);
}

void process_readings(Engine *engine)
{
    if (engine->queue.count == 0) {
        printf("No readings waiting. Generate some first.\n");
        return;
    }
    printf("\nProcessing readings through callbacks:\n");
    ProcessResult r = process_queue(&engine->devices, &engine->queue, &engine->monitor);
    engine->skipped_readings += r.skipped;
    printf("Processed %zu readings", r.processed);
    if (r.skipped > 0)
        printf(", skipped %zu for removed devices", r.skipped);
    printf(".\n");
}

void display_alerts(Engine *engine)
{
    const Monitor *m = &engine->monitor;
    printf("\nAlerts (%zu):\n", m->alert_count);
    if (m->alert_count == 0) {
        printf("No alerts so far.\n");
        return;
    }
    for (const Alert *a = m->alerts, *end = a + m->alert_count; a < end; a++) {
        const DeviceLimits *l = device_limits(a->type);
        printf("[%zu] Device %d \"%s\" (%s)\n", (size_t)(a - m->alerts) + 1, a->device_id,
               a->device_name, l->type_name);
        printf("     Reading: %.2f %s | Expected: %.2f to %.2f %s\n", a->value, l->unit,
               a->expected_low, a->expected_high, l->unit);
        printf("     Reason: %s\n", a->reason);
        printf("     Callback: %s\n", a->callback);
    }
}

void monitoring_summary(Engine *engine)
{
    const Monitor *m = &engine->monitor;
    printf("\nMonitoring Summary\n");
    printf("Registered devices: %zu\n", engine->devices.count);
    printf("Readings processed: %zu\n", m->readings_processed);
    printf("Normal readings: %zu\n", m->normal_readings);
    printf("Abnormal readings: %zu\n", m->abnormal_readings);
    printf("Readings still waiting: %zu\n", engine->queue.count);
    printf("Readings skipped (device removed): %zu\n", engine->skipped_readings);

    printf("\nOut-of-range alerts per device type:\n");
    for (int t = 0; t < DEVICE_TYPE_COUNT; t++)
        printf("  %-12s %zu\n", device_limits((DeviceType)t)->type_name,
               m->range_alerts_per_type[t]);

    printf("\nCustom callback - Sensor Drift Detector:\n");
    printf("  Drift checks: %zu, drift alerts: %zu\n", m->drift_checks, m->drift_events);
    for (int t = 0; t < DEVICE_TYPE_COUNT; t++)
        printf("  %-12s %zu (limit %.2f %s per reading)\n",
               device_limits((DeviceType)t)->type_name, m->drift_per_type[t],
               device_limits((DeviceType)t)->drift_limit, device_limits((DeviceType)t)->unit);
    if (m->largest_drift_device != 0)
        printf("  Largest jump: device %d changed by %+.2f (%.0f%% of its limit)\n",
               m->largest_drift_device, m->largest_drift_change, m->largest_drift_ratio * 100.0);
    printf("Total alerts logged: %zu\n", m->alert_count);
}
