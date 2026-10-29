#ifndef SIMULATION_H
#define SIMULATION_H

#include "monitor.h"
#include "registry.h"

// A generated reading waiting to be processed. It stores the device id, not a
// pointer, so removing devices or moving the array never leaves it dangling.
typedef struct PendingReading {
    int device_id;
    DeviceType type;
    union DeviceReading value;
} PendingReading;

// Growable queue of pending readings.
typedef struct ReadingQueue {
    PendingReading *items;
    size_t count;
    size_t capacity;
} ReadingQueue;

void queue_init(ReadingQueue *queue);
void queue_free(ReadingQueue *queue);

// Results of one processing pass.
typedef struct ProcessResult {
    size_t processed;
    size_t skipped; // readings whose device was removed before processing
} ProcessResult;

// A random reading for this type, inside its documented simulation range.
union DeviceReading generate_reading(DeviceType type);

// Queues one new reading per registered device, `rounds` times.
// Returns the number queued, or -1 if memory ran out (readings queued so far are kept).
long simulate_rounds(const DeviceList *devices, ReadingQueue *queue, int rounds);

// Copies each queued reading into its device and runs the type callback, then the
// drift detector, through process_device. Prints one line per reading. Empties the queue.
ProcessResult process_queue(DeviceList *devices, ReadingQueue *queue, Monitor *monitor);

#endif
