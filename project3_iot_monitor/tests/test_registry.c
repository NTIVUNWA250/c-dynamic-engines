#include "registry.h"
#include "test.h"

#include <string.h>

int main(void)
{
    DeviceList list;
    CHECK(devices_init(&list, 2));

    CHECK(devices_register(&list, 1, "Boiler room", DEVICE_TEMPERATURE) == DEV_OK);
    CHECK(devices_register(&list, 2, "Main pipe", DEVICE_PRESSURE) == DEV_OK);
    CHECK(devices_register(&list, 3, "Gateway battery", DEVICE_BATTERY) == DEV_OK);
    CHECK(list.count == 3 && list.capacity >= 3);

    CHECK(devices_register(&list, 1, "Copy", DEVICE_BATTERY) == DEV_DUPLICATE_ID);
    CHECK(devices_register(&list, 0, "Bad", DEVICE_BATTERY) == DEV_INVALID_ID);
    CHECK(devices_register(&list, 100000, "Bad", DEVICE_BATTERY) == DEV_INVALID_ID);
    CHECK(devices_register(&list, 9, "Bad", DEVICE_TYPE_COUNT) == DEV_INVALID_TYPE);
    CHECK(devices_register(&list, 9, "", DEVICE_BATTERY) == DEV_EMPTY_NAME);
    CHECK(list.count == 3);

    CHECK(devices_find(&list, 1)->status == STATUS_IDLE);
    CHECK(devices_register(&list, 4, "A very long device name that does not fit in forty chars",
                           DEVICE_TEMPERATURE) == DEV_OK);
    CHECK(strlen(devices_find(&list, 4)->name) == DEVICE_NAME_LEN - 1);

    Device *t = devices_find(&list, 1);
    t->readings.temperature = 38.5f;
    CHECK(device_value(t) == 38.5);
    Device *p = devices_find(&list, 2);
    p->readings.pressure = 1013;
    CHECK(device_value(p) == 1013.0);
    char text[32];
    format_reading(p, text, sizeof text);
    CHECK(strcmp(text, "1013 hPa") == 0);
    Device *b = devices_find(&list, 3);
    b->readings.voltage = 3.70f;
    format_reading(b, text, sizeof text);
    CHECK(strcmp(text, "3.70 V") == 0);

    CHECK(devices_remove(&list, 2));
    CHECK(!devices_remove(&list, 2));
    CHECK(list.count == 3 && list.items[0].id == 1 && list.items[1].id == 3);
    CHECK(devices_find(&list, 3)->readings.voltage == 3.70f);

    for (int id = 100; id < 140; id++)
        CHECK(devices_register(&list, id, "Load", DEVICE_BATTERY) == DEV_OK);
    size_t big = list.capacity;
    for (int id = 100; id < 140; id++)
        CHECK(devices_remove(&list, id));
    CHECK(list.capacity < big && list.count == 3);

    CHECK(devices_remove(&list, 1) && devices_remove(&list, 3) && devices_remove(&list, 4));
    CHECK(list.items == NULL && list.capacity == 0);

    CHECK(device_limits(DEVICE_TYPE_COUNT) == NULL);
    CHECK(strcmp(device_limits(DEVICE_PRESSURE)->unit, "hPa") == 0);

    devices_free(&list);
    return TEST_DONE();
}
