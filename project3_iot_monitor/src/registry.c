#include "registry.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_CAPACITY 4

int devices_init(DeviceList *list, size_t initial_capacity)
{
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
    if (initial_capacity == 0)
        return 1;
    list->items = calloc(initial_capacity, sizeof *list->items);
    if (list->items == NULL) {
        log_msg(LOG_ERROR, "registry", "could not allocate %zu devices", initial_capacity);
        return 0;
    }
    list->capacity = initial_capacity;
    return 1;
}

void devices_free(DeviceList *list)
{
    free(list->items);
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

static int resize(DeviceList *list, size_t new_capacity)
{
    Device *moved = realloc(list->items, new_capacity * sizeof *moved);
    if (moved == NULL) {
        log_msg(LOG_ERROR, "registry", "realloc to %zu devices failed", new_capacity);
        return 0;
    }
    log_msg(LOG_INFO, "registry", "capacity %zu -> %zu", list->capacity, new_capacity);
    list->items = moved;
    list->capacity = new_capacity;
    return 1;
}

Device *devices_find(const DeviceList *list, int id)
{
    for (Device *p = list->items, *end = list->items + list->count; p < end; p++)
        if (p->id == id)
            return p;
    return NULL;
}

RegisterResult devices_register(DeviceList *list, int id, const char *name, DeviceType type)
{
    if (id < DEVICE_ID_MIN || id > DEVICE_ID_MAX)
        return DEV_INVALID_ID;
    if (device_limits(type) == NULL)
        return DEV_INVALID_TYPE;
    if (name == NULL || name[0] == '\0')
        return DEV_EMPTY_NAME;
    if (devices_find(list, id) != NULL)
        return DEV_DUPLICATE_ID;

    if (list->count == list->capacity) {
        size_t new_capacity = list->capacity < MIN_CAPACITY ? MIN_CAPACITY : list->capacity * 2;
        if (!resize(list, new_capacity))
            return DEV_NO_MEMORY;
    }

    Device *slot = list->items + list->count;
    memset(slot, 0, sizeof *slot);
    slot->id = id;
    snprintf(slot->name, sizeof slot->name, "%s", name);
    slot->type = type;
    slot->status = STATUS_IDLE;
    list->count++;
    log_msg(LOG_INFO, "registry", "registered %d (%s)", id, device_limits(type)->type_name);
    return DEV_OK;
}

int devices_remove(DeviceList *list, int id)
{
    Device *target = devices_find(list, id);
    if (target == NULL)
        return 0;

    Device *end = list->items + list->count;
    memmove(target, target + 1, (size_t)(end - target - 1) * sizeof *target);
    list->count--;
    log_msg(LOG_INFO, "registry", "removed %d (%zu left)", id, list->count);

    if (list->count == 0) {
        devices_free(list);
    } else if (list->capacity > MIN_CAPACITY && list->count <= list->capacity / 4) {
        resize(list, list->capacity / 2);
    }
    return 1;
}

const char *register_result_text(RegisterResult result)
{
    switch (result) {
    case DEV_OK:
        return "device registered";
    case DEV_DUPLICATE_ID:
        return "a device with this ID already exists";
    case DEV_INVALID_ID:
        return "ID must be between 1 and 99999";
    case DEV_INVALID_TYPE:
        return "unknown device type";
    case DEV_EMPTY_NAME:
        return "name cannot be empty";
    case DEV_NO_MEMORY:
        return "not enough memory to register the device";
    }
    return "unknown result";
}
