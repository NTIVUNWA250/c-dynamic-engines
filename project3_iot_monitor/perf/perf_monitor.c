#include "monitor.h"
#include "registry.h"
#include "simulation.h"
#include "test.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEVICES 2000 // registering scans for duplicate IDs, so n registrations cost O(n^2)
#define ROUNDS 50    // 100,000 readings
#define BUDGET_SECONDS 1.0

static double seconds_since(clock_t start)
{
    return (double)(clock() - start) / CLOCKS_PER_SEC;
}

// Times one step and fails the check if it goes over the budget.
#define TIMED(label, code)                                     \
    do {                                                       \
        clock_t start_ = clock();                              \
        code;                                                  \
        double took_ = seconds_since(start_);                  \
        fprintf(stderr, "%-34s %.3fs\n", label, took_);        \
        CHECK(took_ < BUDGET_SECONDS);                         \
    } while (0)

// Large-input timing: registering, simulating and processing 100k readings.
int main(void)
{
    srand(1);
    DeviceList devices;
    CHECK(devices_init(&devices, 0));
    TIMED("register 2k devices", for (int id = 1; id <= DEVICES; id++)
                                     devices_register(&devices, id, "Load", (DeviceType)(id % 3)));
    CHECK(devices.count == DEVICES);

    ReadingQueue queue;
    queue_init(&queue);
    TIMED("simulate 100k readings", simulate_rounds(&devices, &queue, ROUNDS));
    CHECK(queue.count == (size_t)DEVICES * ROUNDS);

    Monitor monitor;
    monitor_init(&monitor);
    // stdout goes to /dev/null so printing does not dominate the timing.
    FILE *saved = freopen("/dev/null", "w", stdout);
    CHECK(saved != NULL);
    TIMED("process 100k through callbacks", process_queue(&devices, &queue, &monitor));
    CHECK(monitor.readings_processed == (size_t)DEVICES * ROUNDS);
    CHECK(monitor.normal_readings + monitor.abnormal_readings == monitor.readings_processed);
    fprintf(stderr, "normal %zu, abnormal %zu, drift alerts %zu\n", monitor.normal_readings,
            monitor.abnormal_readings, monitor.drift_events);

    monitor_free(&monitor);
    queue_free(&queue);
    devices_free(&devices);
    int failed = test_failures != 0;
    fprintf(stderr, "%d checks, %d failed\n", test_checks, test_failures);
    return failed;
}
