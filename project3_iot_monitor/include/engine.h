#ifndef ENGINE_H
#define ENGINE_H

#include "monitor.h"
#include "registry.h"
#include "simulation.h"

typedef struct Engine {
    DeviceList devices;
    ReadingQueue queue;
    Monitor monitor;
    size_t skipped_readings;
} Engine;

int engine_init(Engine *engine);

void engine_free(Engine *engine);

typedef void (*EngineOperation)(Engine *engine);

void register_device(Engine *engine);
void display_devices(Engine *engine);
void remove_device(Engine *engine);
void generate_readings(Engine *engine);
void process_readings(Engine *engine);
void display_alerts(Engine *engine);
void monitoring_summary(Engine *engine);

#endif
