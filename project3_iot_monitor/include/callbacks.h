#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "device.h"

// A monitoring callback: inspects one device's current reading.
typedef void (*DeviceCallback)(struct Device *device);

// Runs callback on device. Does nothing if either pointer is NULL.
void process_device(struct Device *device, void (*callback)(struct Device *));

// Range checks, one per device type. Each reads only its own union member.
void temperature_monitor(struct Device *device);
void pressure_monitor(struct Device *device);
void battery_monitor(struct Device *device);

// Custom callback: flags a reading that jumps too far from the previous one (src/drift.c).
void drift_detector(struct Device *device);

// The range-check callback for a type, picked from a table, and its name.
DeviceCallback callback_for_type(DeviceType type);
const char *callback_name_for_type(DeviceType type);

#endif
