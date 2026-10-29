#include "device.h"

#include <stdio.h>

static const DeviceLimits LIMITS[DEVICE_TYPE_COUNT] = {
    [DEVICE_TEMPERATURE] = {"Temperature", "C", 5.0, 60.0, 15.0, 45.0, 10.0, 6.0},
    [DEVICE_PRESSURE] = {"Pressure", "hPa", 850.0, 1150.0, 950.0, 1050.0, 60.0, 35.0},
    [DEVICE_BATTERY] = {"Battery", "V", 2.9, 4.4, 3.3, 4.2, 0.35, 0.2},
};

const DeviceLimits *device_limits(DeviceType type)
{
    return (type >= 0 && type < DEVICE_TYPE_COUNT) ? &LIMITS[type] : NULL;
}

double device_value(const Device *device)
{
    switch (device->type) {
    case DEVICE_TEMPERATURE:
        return device->readings.temperature;
    case DEVICE_PRESSURE:
        return device->readings.pressure;
    case DEVICE_BATTERY:
        return device->readings.voltage;
    default:
        return 0.0;
    }
}

void format_reading(const Device *device, char *buf, size_t size)
{
    switch (device->type) {
    case DEVICE_TEMPERATURE:
        snprintf(buf, size, "%.1f C", device->readings.temperature);
        break;
    case DEVICE_PRESSURE:
        snprintf(buf, size, "%d hPa", device->readings.pressure);
        break;
    case DEVICE_BATTERY:
        snprintf(buf, size, "%.2f V", device->readings.voltage);
        break;
    default:
        snprintf(buf, size, "?");
    }
}

const char *device_status_name(DeviceStatus status)
{
    static const char *NAMES[] = {"IDLE", "NORMAL", "ALERT"};
    return status <= STATUS_ALERT ? NAMES[status] : "?";
}
