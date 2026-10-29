#ifndef REGISTRY_H
#define REGISTRY_H

#include "device.h"

// Growable array of registered devices on the heap.
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

// Allocates room for initial_capacity devices with calloc. Returns 1 on success.
int devices_init(DeviceList *list, size_t initial_capacity);

// Frees the array and resets the list.
void devices_free(DeviceList *list);

// Validates and adds a new idle device, growing the array with realloc if full.
RegisterResult devices_register(DeviceList *list, int id, const char *name, DeviceType type);

// Removes a device and shrinks the array when it is mostly empty. Returns 1 if removed.
int devices_remove(DeviceList *list, int id);

// Returns the device with this id, or NULL.
Device *devices_find(const DeviceList *list, int id);

const char *register_result_text(RegisterResult result);

#endif
