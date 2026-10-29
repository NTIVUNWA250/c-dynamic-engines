#include "simulation.h"
#include "test.h"

#include <stdio.h>
#include <stdlib.h>

// Generated values stay in range, readings are queued and processed, removed devices skipped.
int main(void)
{
    srand(12345);

    // Simulated values stay in range, start normal, and step by at most 2.5 drift limits.
    DeviceList walk;
    devices_init(&walk, 0);
    devices_register(&walk, 1, "T", DEVICE_TEMPERATURE);
    devices_register(&walk, 2, "P", DEVICE_PRESSURE);
    devices_register(&walk, 3, "B", DEVICE_BATTERY);
    Device *t = devices_find(&walk, 1);
    Device *p = devices_find(&walk, 2);
    Device *b = devices_find(&walk, 3);
    union DeviceReading first = simulate_next(t);
    CHECK(first.temperature >= 15.0f && first.temperature <= 45.0f);
    double last_t = first.temperature;
    for (int i = 0; i < 2000; i++) {
        union DeviceReading rt = simulate_next(t);
        union DeviceReading rp = simulate_next(p);
        union DeviceReading rb = simulate_next(b);
        CHECK(rt.temperature >= 5.0f && rt.temperature <= 60.0f);
        CHECK(rp.pressure >= 850 && rp.pressure <= 1150);
        CHECK(rb.voltage >= 2.9f && rb.voltage <= 4.4f);
        CHECK(rt.temperature - last_t <= 25.01 && last_t - rt.temperature <= 25.01);
        last_t = rt.temperature;
    }
    devices_free(&walk);

    DeviceList devices;
    devices_init(&devices, 0);
    devices_register(&devices, 1, "T1", DEVICE_TEMPERATURE);
    devices_register(&devices, 2, "P1", DEVICE_PRESSURE);
    devices_register(&devices, 3, "B1", DEVICE_BATTERY);
    devices_register(&devices, 4, "T2", DEVICE_TEMPERATURE);

    ReadingQueue queue;
    queue_init(&queue);
    Monitor monitor;
    monitor_init(&monitor);

    // 4 devices x 3 rounds = 12 readings, more than the required 10.
    CHECK(simulate_rounds(&devices, &queue, 3) == 12);
    CHECK(queue.count == 12);
    CHECK(queue.items[0].device_id == 1 && queue.items[4].device_id == 1);

    // Remove a device with readings still queued: its 3 readings are skipped safely.
    CHECK(devices_remove(&devices, 2));
    ProcessResult r = process_queue(&devices, &queue, &monitor);
    CHECK(r.processed == 9 && r.skipped == 3);
    CHECK(queue.count == 0);
    CHECK(monitor.readings_processed == 9);
    CHECK(monitor.normal_readings + monitor.abnormal_readings == 9);
    CHECK(monitor.drift_checks == 9);
    CHECK(devices_find(&devices, 1)->reading_count == 3);
    CHECK(devices_find(&devices, 1)->status != STATUS_IDLE);

    // Processing an empty queue does nothing.
    r = process_queue(&devices, &queue, &monitor);
    CHECK(r.processed == 0 && r.skipped == 0);

    queue_free(&queue);
    monitor_free(&monitor);
    devices_free(&devices);
    return TEST_DONE();
}
