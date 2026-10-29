#include "monitor.h"
#include "registry.h"
#include "simulation.h"
#include "test.h"
#include "test_alloc.h"

// When calloc/realloc fail, the engine reports it and keeps its existing data.
int main(void)
{
    DeviceList list;
    test_alloc_fail_after(0);
    CHECK(!devices_init(&list, 6));
    CHECK(list.items == NULL);

    CHECK(devices_init(&list, 1));
    CHECK(devices_register(&list, 1, "First", DEVICE_BATTERY) == DEV_OK);
    test_alloc_fail_after(0); // the array is full, so this register needs realloc
    CHECK(devices_register(&list, 2, "Second", DEVICE_BATTERY) == DEV_NO_MEMORY);
    CHECK(list.count == 1 && list.items[0].id == 1);
    CHECK(devices_register(&list, 2, "Second", DEVICE_BATTERY) == DEV_OK);

    // Reading queue: a failed push stops the simulation with -1.
    ReadingQueue queue;
    queue_init(&queue);
    test_alloc_fail_after(0);
    CHECK(simulate_rounds(&list, &queue, 1) == -1);
    CHECK(queue.count == 0);
    CHECK(simulate_rounds(&list, &queue, 1) == 2);

    // Alert log: a failed append is reported and nothing is half-written.
    Monitor m;
    monitor_init(&m);
    test_alloc_fail_after(0);
    CHECK(!monitor_add_alert(&m, &list.items[0], 0.0, 1.0, "test", "test"));
    CHECK(m.alert_count == 0);
    CHECK(monitor_add_alert(&m, &list.items[0], 0.0, 1.0, "test", "test"));
    CHECK(m.alert_count == 1);

    monitor_free(&m);
    queue_free(&queue);
    devices_free(&list);
    return TEST_DONE();
}
