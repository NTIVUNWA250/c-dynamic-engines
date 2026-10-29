#include "monitor.h"
#include "log.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Monitor *active_monitor = NULL;

int monitor_init(Monitor *monitor)
{
    memset(monitor, 0, sizeof *monitor);
    return 1;
}

void monitor_free(Monitor *monitor)
{
    free(monitor->alerts);
    if (active_monitor == monitor)
        active_monitor = NULL;
    memset(monitor, 0, sizeof *monitor);
}

void monitor_bind(Monitor *monitor)
{
    active_monitor = monitor;
}

Monitor *monitor_active(void)
{
    return active_monitor;
}

int monitor_add_alert(Monitor *monitor, const Device *device, double low, double high,
                      const char *reason, const char *callback)
{
    if (monitor->alert_count == monitor->alert_capacity) {
        size_t new_capacity = monitor->alert_capacity == 0 ? 8 : monitor->alert_capacity * 2;
        Alert *grown = realloc(monitor->alerts, new_capacity * sizeof *grown);
        if (grown == NULL) {
            log_msg(LOG_ERROR, "monitor", "could not grow alert log to %zu", new_capacity);
            return 0;
        }
        monitor->alerts = grown;
        monitor->alert_capacity = new_capacity;
    }

    Alert *alert = monitor->alerts + monitor->alert_count++;
    alert->device_id = device->id;
    snprintf(alert->device_name, sizeof alert->device_name, "%s", device->name);
    alert->type = device->type;
    alert->value = device_value(device);
    alert->expected_low = low;
    alert->expected_high = high;
    snprintf(alert->reason, sizeof alert->reason, "%s", reason);
    alert->callback = callback;
    log_msg(LOG_INFO, "monitor", "alert on device %d: %s", device->id, reason);
    return 1;
}
