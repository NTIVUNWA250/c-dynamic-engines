#ifndef DEVICE_H
#define DEVICE_H

#include <stddef.h>

#define DEVICE_NAME_LEN 40

typedef enum { DEVICE_TEMPERATURE, DEVICE_PRESSURE, DEVICE_BATTERY, DEVICE_TYPE_COUNT } DeviceType;

typedef enum { STATUS_IDLE, STATUS_NORMAL, STATUS_ALERT } DeviceStatus;

union DeviceReading {
    float temperature;
    int pressure;
    float voltage;
};

struct Device {
    int id;
    char name[DEVICE_NAME_LEN];
    DeviceType type;
    union DeviceReading readings;
    DeviceStatus status;
    size_t reading_count;
    int has_previous;
    double previous;
    int has_sim_last;
    double sim_last;
};
typedef struct Device Device;

typedef struct DeviceLimits {
    const char *type_name;
    const char *unit;
    double sim_min, sim_max;
    double normal_low, normal_high;
    double drift_limit;
    double step;
} DeviceLimits;

const DeviceLimits *device_limits(DeviceType type);

double device_value(const Device *device);

void format_reading(const Device *device, char *buf, size_t size);

const char *device_status_name(DeviceStatus status);

#endif
