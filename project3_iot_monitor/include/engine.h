#ifndef ENGINE_H
#define ENGINE_H

#include "monitor.h"
#include "registry.h"
#include "simulation.h"

// Everything the menu operations work on.
typedef struct Engine {
    DeviceList devices;
    ReadingQueue queue;
    Monitor monitor;
    size_t skipped_readings; // queued readings dropped because their device was removed
} Engine;

// Allocates the device array and registers the starting devices. Returns 1 on success.
int engine_init(Engine *engine);

// Frees the devices, queue and alert log.
void engine_free(Engine *engine);

// Every menu operation has this shape, so they can be dispatched from one table.
typedef void (*EngineOperation)(Engine *engine);

void register_device(Engine *engine);
void display_devices(Engine *engine);
void remove_device(Engine *engine);
void generate_readings(Engine *engine);
void process_readings(Engine *engine);
void display_alerts(Engine *engine);
void monitoring_summary(Engine *engine);

#endif
