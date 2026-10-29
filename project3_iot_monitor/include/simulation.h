#ifndef SIMULATION_H
#define SIMULATION_H

#include "monitor.h"
#include "registry.h"

typedef struct PendingReading {
    int device_id;
    DeviceType type;
    union DeviceReading value;
} PendingReading;

typedef struct ReadingQueue {
    PendingReading *items;
    size_t count;
    size_t capacity;
} ReadingQueue;

void queue_init(ReadingQueue *queue);
void queue_free(ReadingQueue *queue);

typedef struct ProcessResult {
    size_t processed;
    size_t skipped;
} ProcessResult;

#define SPIKE_PERCENT 15

union DeviceReading simulate_next(Device *device);

long simulate_rounds(DeviceList *devices, ReadingQueue *queue, int rounds);

ProcessResult process_queue(DeviceList *devices, ReadingQueue *queue, Monitor *monitor);

#endif
