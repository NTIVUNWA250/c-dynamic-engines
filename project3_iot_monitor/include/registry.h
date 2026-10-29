#ifndef REGISTRY_H
#define REGISTRY_H

#include "device.h"

typedef struct DeviceList {
    Device *items;
    size_t count;
    size_t capacity;
} DeviceList;

typedef enum {
    DEV_OK,
    DEV_DUPLICATE_ID,
    DEV_INVALID_ID,
    DEV_INVALID_TYPE,
    DEV_EMPTY_NAME,
    DEV_NO_MEMORY
} RegisterResult;

#define DEVICE_ID_MIN 1
#define DEVICE_ID_MAX 99999

int devices_init(DeviceList *list, size_t initial_capacity);

void devices_free(DeviceList *list);

RegisterResult devices_register(DeviceList *list, int id, const char *name, DeviceType type);

int devices_remove(DeviceList *list, int id);

Device *devices_find(const DeviceList *list, int id);

const char *register_result_text(RegisterResult result);

#endif
