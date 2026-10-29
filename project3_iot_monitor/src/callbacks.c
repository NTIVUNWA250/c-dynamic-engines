#include "callbacks.h"
#include "log.h"
#include "monitor.h"

#include <stdio.h>

void process_device(struct Device *device, void (*callback)(struct Device *))
{
    if (device == NULL || callback == NULL)
        return;
    log_msg(LOG_DEBUG, "callbacks", "dispatching device %d", device->id);
    callback(device);
}

static void check_range(struct Device *device, double value, const char *callback,
                        const char *too_low, const char *too_high)
{
    Monitor *m = monitor_active();
    const DeviceLimits *limits = device_limits(device->type);
    if (m == NULL || limits == NULL) {
        log_msg(LOG_ERROR, "callbacks", "no monitor bound or bad type");
        return;
    }
    m->readings_processed++;
    device->reading_count++;

    if (value >= limits->normal_low && value <= limits->normal_high) {
        device->status = STATUS_NORMAL;
        m->normal_readings++;
        snprintf(m->last_note, sizeof m->last_note, "NORMAL (range %g-%g %s)",
                 limits->normal_low, limits->normal_high, limits->unit);
        return;
    }

    const char *reason = value < limits->normal_low ? too_low : too_high;
    device->status = STATUS_ALERT;
    m->abnormal_readings++;
    m->range_alerts_per_type[device->type]++;
    monitor_add_alert(m, device, limits->normal_low, limits->normal_high, reason, callback);
    snprintf(m->last_note, sizeof m->last_note, "ALERT: %s", reason);
}

void temperature_monitor(struct Device *device)
{
    check_range(device, device->readings.temperature, "temperature_monitor",
                "too cold: below the safe operating range",
                "overheating: above the safe operating range");
}

void pressure_monitor(struct Device *device)
{
    check_range(device, device->readings.pressure, "pressure_monitor",
                "under-pressure: possible leak or pump failure",
                "over-pressure: risk of pipe or seal damage");
}

void battery_monitor(struct Device *device)
{
    check_range(device, device->readings.voltage, "battery_monitor",
                "low voltage: battery nearly flat, device may shut down",
                "over-voltage: charging fault, battery may be damaged");
}

static const struct {
    DeviceCallback callback;
    const char *name;
} TYPE_CALLBACKS[DEVICE_TYPE_COUNT] = {
    [DEVICE_TEMPERATURE] = {temperature_monitor, "temperature_monitor"},
    [DEVICE_PRESSURE] = {pressure_monitor, "pressure_monitor"},
    [DEVICE_BATTERY] = {battery_monitor, "battery_monitor"},
};

DeviceCallback callback_for_type(DeviceType type)
{
    return device_limits(type) != NULL ? TYPE_CALLBACKS[type].callback : NULL;
}

const char *callback_name_for_type(DeviceType type)
{
    return device_limits(type) != NULL ? TYPE_CALLBACKS[type].name : "none";
}
