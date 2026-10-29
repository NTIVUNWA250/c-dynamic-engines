#ifndef DEVICE_H
#define DEVICE_H

#include <stddef.h>

#define DEVICE_NAME_LEN 40

typedef enum { DEVICE_TEMPERATURE, DEVICE_PRESSURE, DEVICE_BATTERY, DEVICE_TYPE_COUNT } DeviceType;

typedef enum { STATUS_IDLE, STATUS_NORMAL, STATUS_ALERT } DeviceStatus;

// One reading. Only the member that matches the device type is ever valid.
union DeviceReading {
    float temperature; // degrees Celsius
    int pressure;      // hectopascals (hPa)
    float voltage;     // volts
};

struct Device {
    int id;
    char name[DEVICE_NAME_LEN];
    DeviceType type;
    union DeviceReading readings;
    DeviceStatus status;
    size_t reading_count;  // readings processed for this device
    int has_previous;      // 1 once a reading has been seen (for drift detection)
    double previous;       // the reading before the current one
    int has_sim_last;      // simulator state: 1 once a value has been generated
    double sim_last;       // simulator state: the last generated value
};
typedef struct Device Device;

// Operating limits and simulation range for one device type.
typedef struct DeviceLimits {
    const char *type_name;
    const char *unit;
    double sim_min, sim_max;        // simulated readings fall in this range
    double normal_low, normal_high; // readings outside this range raise an alert
    double drift_limit;             // largest normal change between two readings
    double step;                    // largest normal step the simulator takes per reading
} DeviceLimits;

// Limits for a device type, or NULL if the type is not valid.
const DeviceLimits *device_limits(DeviceType type);

// The current reading as a double, read from the union member for this type.
double device_value(const Device *device);

// Writes the current reading with its unit, e.g. "38.4 C", "1012 hPa", "3.71 V".
void format_reading(const Device *device, char *buf, size_t size);

const char *device_status_name(DeviceStatus status);

#endif
