#include "engine.h"
#include "log.h"

static const struct {
    int id;
    const char *name;
    DeviceType type;
} STARTING_DEVICES[] = {
    {101, "Server room temp", DEVICE_TEMPERATURE},
    {102, "Cold store temp", DEVICE_TEMPERATURE},
    {201, "Water main pressure", DEVICE_PRESSURE},
    {202, "Boiler pressure", DEVICE_PRESSURE},
    {301, "Gateway battery", DEVICE_BATTERY},
    {302, "Field node battery", DEVICE_BATTERY},
};

#define STARTING_COUNT (sizeof STARTING_DEVICES / sizeof STARTING_DEVICES[0])

int engine_init(Engine *engine)
{
    engine->skipped_readings = 0;
    queue_init(&engine->queue);
    monitor_init(&engine->monitor);
    if (!devices_init(&engine->devices, STARTING_COUNT))
        return 0;
    for (size_t i = 0; i < STARTING_COUNT; i++) {
        if (devices_register(&engine->devices, STARTING_DEVICES[i].id, STARTING_DEVICES[i].name,
                             STARTING_DEVICES[i].type) != DEV_OK) {
            log_msg(LOG_ERROR, "engine", "could not register device %d", STARTING_DEVICES[i].id);
            engine_free(engine);
            return 0;
        }
    }
    monitor_bind(&engine->monitor);
    return 1;
}

void engine_free(Engine *engine)
{
    devices_free(&engine->devices);
    queue_free(&engine->queue);
    monitor_free(&engine->monitor);
}
