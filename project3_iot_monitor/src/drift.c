#include "callbacks.h"
#include "log.h"
#include "monitor.h"

#include <math.h>
#include <stdio.h>

// Sensor Drift Detector (see docs/custom_callback.md):
// change = current - previous. If |change| > the type's drift_limit, raise an alert.
// The first reading of a device only sets the baseline.
void drift_detector(struct Device *device)
{
    Monitor *m = monitor_active();
    const DeviceLimits *limits = device_limits(device->type);
    if (m == NULL || limits == NULL)
        return;

    double value = device_value(device);
    m->drift_checks++;
    m->drift_note[0] = '\0';

    if (device->has_previous) {
        double change = value - device->previous;
        double ratio = fabs(change) / limits->drift_limit;
        // Remember the biggest jump seen, measured against its own type's limit.
        if (ratio > m->largest_drift_ratio) {
            m->largest_drift_ratio = ratio;
            m->largest_drift_device = device->id;
            m->largest_drift_change = change;
        }
        if (fabs(change) > limits->drift_limit) {
            char reason[REASON_LEN];
            snprintf(reason, sizeof reason, "sudden %s of %.2f %s since the last reading",
                     change > 0 ? "rise" : "drop", fabs(change), limits->unit);
            device->status = STATUS_ALERT;
            m->drift_events++;
            m->drift_per_type[device->type]++;
            monitor_add_alert(m, device, device->previous - limits->drift_limit,
                              device->previous + limits->drift_limit, reason,
                              "drift_detector");
            int written = snprintf(m->drift_note, sizeof m->drift_note,
                                   "DRIFT: %s (limit %.2f)", reason, limits->drift_limit);
            if (written >= (int)sizeof m->drift_note)
                log_msg(LOG_DEBUG, "drift", "note shortened to fit");
        }
    }
    device->previous = value;
    device->has_previous = 1;
}
