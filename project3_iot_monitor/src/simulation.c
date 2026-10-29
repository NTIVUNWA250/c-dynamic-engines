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

static double random_between(double min, double max)
{
    return min + (max - min) * ((double)rand() / (double)RAND_MAX);
}

static union DeviceReading pack_temperature(double value)
{
    union DeviceReading r;
    r.temperature = (float)value;
    return r;
}

static union DeviceReading pack_pressure(double value)
{
    union DeviceReading r;
    r.pressure = (int)(value + 0.5);
    return r;
}

static union DeviceReading pack_battery(double value)
{
    union DeviceReading r;
    r.voltage = (float)value;
    return r;
}

static union DeviceReading (*const PACKERS[DEVICE_TYPE_COUNT])(double) = {
    [DEVICE_TEMPERATURE] = pack_temperature,
    [DEVICE_PRESSURE] = pack_pressure,
    [DEVICE_BATTERY] = pack_battery,
};

union DeviceReading simulate_next(Device *device)
{
    const DeviceLimits *l = device_limits(device->type);
    double value;
    if (!device->has_sim_last) {
        value = random_between(l->normal_low, l->normal_high);
    } else if (rand() % 100 < SPIKE_PERCENT) {
        double jump = random_between(1.2, 2.5) * l->drift_limit;
        value = device->sim_last + (rand() % 2 ? jump : -jump);
    } else {
        double centre = (l->normal_low + l->normal_high) / 2.0;
        value = device->sim_last + 0.15 * (centre - device->sim_last) +
                random_between(-l->step, l->step);
    }
    if (value < l->sim_min)
        value = l->sim_min;
    if (value > l->sim_max)
        value = l->sim_max;
    device->sim_last = value;
    device->has_sim_last = 1;
    return PACKERS[device->type](value);
}

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

long simulate_rounds(DeviceList *devices, ReadingQueue *queue, int rounds)
{
    long queued = 0;
    for (int round = 0; round < rounds; round++) {
        for (Device *d = devices->items, *end = d + devices->count; d < end; d++) {
            PendingReading reading = {d->id, d->type, simulate_next(d)};
            if (!queue_push(queue, &reading))
                return -1;
            queued++;
        }
    }
    log_msg(LOG_INFO, "simulation", "queued %ld readings", queued);
    return queued;
}

static void print_result(size_t number, const Device *d, const Monitor *m)
{
    char value[32];
    format_reading(d, value, sizeof value);
    printf("%-4zu %-6d %-24.24s %-12s %-10s %-20s %s\n", number, d->id, d->name,
           device_limits(d->type)->type_name, value, callback_name_for_type(d->type),
           m->last_note);
    if (m->drift_note[0] != '\0')
        printf("%-60s drift_detector       %s\n", "", m->drift_note);
}

ProcessResult process_queue(DeviceList *devices, ReadingQueue *queue, Monitor *monitor)
{
    ProcessResult result = {0, 0};
    monitor_bind(monitor);
    if (queue->count > 0)
        printf("%-4s %-6s %-24s %-12s %-10s %-20s %s\n", "#", "ID", "Device", "Type", "Value",
               "Callback", "Result");

    for (const PendingReading *r = queue->items, *end = r + queue->count; r < end; r++) {
        Device *device = devices_find(devices, r->device_id);
        if (device == NULL || device->type != r->type) {
            result.skipped++;
            printf("     reading for device %d skipped: device was removed\n", r->device_id);
            continue;
        }
        device->readings = r->value;
        process_device(device, callback_for_type(device->type));
        process_device(device, drift_detector);
        print_result(++result.processed, device, monitor);
    }
    queue->count = 0;
    log_msg(LOG_INFO, "simulation", "processed %zu, skipped %zu", result.processed,
            result.skipped);
    return result;
}
