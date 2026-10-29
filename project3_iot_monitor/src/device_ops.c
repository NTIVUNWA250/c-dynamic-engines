#include "engine.h"
#include "input.h"

#include <stdio.h>

void register_device(Engine *engine)
{
    int id;
    int type_choice;
    char name[DEVICE_NAME_LEN];
    printf("\n-- Register a device --\n");
    if (!read_int("Device ID (1-99999): ", DEVICE_ID_MIN, DEVICE_ID_MAX, &id))
        return;
    if (devices_find(&engine->devices, id) != NULL) {
        printf("Rejected: a device with ID %d already exists.\n", id);
        return;
    }
    if (!read_text("Device name: ", name, sizeof name))
        return;
    printf("1. Temperature sensor\n2. Pressure sensor\n3. Battery voltage monitor\n");
    if (!read_int("Type: ", 1, DEVICE_TYPE_COUNT, &type_choice))
        return;

    RegisterResult result =
        devices_register(&engine->devices, id, name, (DeviceType)(type_choice - 1));
    if (result == DEV_OK)
        printf("Device %d registered. Total devices: %zu\n", id, engine->devices.count);
    else
        printf("Rejected: %s.\n", register_result_text(result));
}

void display_devices(Engine *engine)
{
    const DeviceList *list = &engine->devices;
    printf("\n-- Registered devices --\n");
    if (list->count == 0) {
        printf("No devices registered.\n");
        return;
    }
    printf("%-6s %-22s %-12s %-12s %-8s %s\n", "ID", "Name", "Type", "Last value", "Status",
           "Readings");
    // Pointer traversal over the device array.
    for (const Device *d = list->items, *end = d + list->count; d < end; d++) {
        char value[32] = "-";
        if (d->reading_count > 0)
            format_reading(d, value, sizeof value);
        printf("%-6d %-22.22s %-12s %-12s %-8s %zu\n", d->id, d->name,
               device_limits(d->type)->type_name, value, device_status_name(d->status),
               d->reading_count);
    }
    printf("Devices: %zu (memory reserved for %zu)\n", list->count, list->capacity);
}

void remove_device(Engine *engine)
{
    int id;
    printf("\n-- Remove a device --\n");
    if (!read_int("Device ID to remove: ", DEVICE_ID_MIN, DEVICE_ID_MAX, &id))
        return;
    if (devices_remove(&engine->devices, id))
        printf("Device %d removed. Total devices: %zu\n", id, engine->devices.count);
    else
        printf("No device with ID %d, nothing removed.\n", id);
}
