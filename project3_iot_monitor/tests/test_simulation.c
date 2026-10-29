#include "simulation.h"
#include "test.h"

#include <stdio.h>
#include <stdlib.h>

// Generated values stay in range, readings are queued and processed, removed devices skipped.
int main(void)
{
    srand(12345);

    // Every generated value lies in its type's simulation range.
    for (int i = 0; i < 2000; i++) {
        union DeviceReading t = generate_reading(DEVICE_TEMPERATURE);
        union DeviceReading p = generate_reading(DEVICE_PRESSURE);
        union DeviceReading b = generate_reading(DEVICE_BATTERY);
        CHECK(t.temperature >= 5.0f && t.temperature <= 60.0f);
        CHECK(p.pressure >= 850 && p.pressure <= 1150);
        CHECK(b.voltage >= 2.9f && b.voltage <= 4.4f);
    }

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
