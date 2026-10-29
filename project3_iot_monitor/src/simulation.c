#include "simulation.h"
#include "callbacks.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>

void queue_init(ReadingQueue *queue)
{
    queue->items = NULL;
    queue->count = 0;
    queue->capacity = 0;
}

void queue_free(ReadingQueue *queue)
{
    free(queue->items);
    queue_init(queue);
}

// Uniform random number in [min, max] using rand().
static double random_between(double min, double max)
{
    return min + (max - min) * ((double)rand() / (double)RAND_MAX);
}

// One generator per type; each fills only the union member for its type.
static union DeviceReading gen_temperature(void)
{
    const DeviceLimits *l = device_limits(DEVICE_TEMPERATURE);
    union DeviceReading r;
    r.temperature = (float)random_between(l->sim_min, l->sim_max);
    return r;
}

static union DeviceReading gen_pressure(void)
{
    const DeviceLimits *l = device_limits(DEVICE_PRESSURE);
    union DeviceReading r;
    r.pressure = (int)(random_between(l->sim_min, l->sim_max) + 0.5);
    return r;
}

static union DeviceReading gen_battery(void)
{
    const DeviceLimits *l = device_limits(DEVICE_BATTERY);
    union DeviceReading r;
    r.voltage = (float)random_between(l->sim_min, l->sim_max);
    return r;
}

static union DeviceReading (*const GENERATORS[DEVICE_TYPE_COUNT])(void) = {
    [DEVICE_TEMPERATURE] = gen_temperature,
    [DEVICE_PRESSURE] = gen_pressure,
    [DEVICE_BATTERY] = gen_battery,
};

union DeviceReading generate_reading(DeviceType type)
{
    return GENERATORS[type]();
}

// Appends a reading, doubling the queue when full. Returns 0 if memory runs out.
static int queue_push(ReadingQueue *queue, const PendingReading *reading)
{
    if (queue->count == queue->capacity) {
        size_t new_capacity = queue->capacity == 0 ? 16 : queue->capacity * 2;
        PendingReading *grown = realloc(queue->items, new_capacity * sizeof *grown);
        if (grown == NULL) {
            log_msg(LOG_ERROR, "simulation", "could not grow queue to %zu", new_capacity);
            return 0;
        }
        queue->items = grown;
        queue->capacity = new_capacity;
    }
    queue->items[queue->count++] = *reading;
    return 1;
}

long simulate_rounds(const DeviceList *devices, ReadingQueue *queue, int rounds)
{
    long queued = 0;
    for (int round = 0; round < rounds; round++) {
        // Pointer traversal over the registered devices.
        for (const Device *d = devices->items, *end = d + devices->count; d < end; d++) {
            PendingReading reading = {d->id, d->type, generate_reading(d->type)};
            if (!queue_push(queue, &reading))
                return -1;
            queued++;
        }
    }
    log_msg(LOG_INFO, "simulation", "queued %ld readings", queued);
    return queued;
}

// Prints one processed reading: who, what value, which callback, and the result.
static void print_result(size_t number, const Device *d, const Monitor *m)
{
    char value[32];
    format_reading(d, value, sizeof value);
    printf("%-4zu %-6d %-20.20s %-12s %-10s %-20s %s\n", number, d->id, d->name,
           device_limits(d->type)->type_name, value, callback_name_for_type(d->type),
           m->last_note);
    if (m->drift_note[0] != '\0')
        printf("%-56s drift_detector       %s\n", "", m->drift_note);
}

ProcessResult process_queue(DeviceList *devices, ReadingQueue *queue, Monitor *monitor)
{
    ProcessResult result = {0, 0};
    monitor_bind(monitor);
    if (queue->count > 0)
        printf("%-4s %-6s %-20s %-12s %-10s %-20s %s\n", "#", "ID", "Device", "Type", "Value",
               "Callback", "Result");

    for (const PendingReading *r = queue->items, *end = r + queue->count; r < end; r++) {
        Device *device = devices_find(devices, r->device_id);
        if (device == NULL || device->type != r->type) {
            result.skipped++;
            printf("     reading for device %d skipped: device was removed\n", r->device_id);
            continue;
        }
        device->readings = r->value; // copy the whole union; the type is unchanged
        process_device(device, callback_for_type(device->type));
        process_device(device, drift_detector);
        print_result(++result.processed, device, monitor);
    }
    queue->count = 0;
    log_msg(LOG_INFO, "simulation", "processed %zu, skipped %zu", result.processed,
            result.skipped);
    return result;
}
