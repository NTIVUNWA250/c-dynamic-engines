#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "device.h"

typedef void (*DeviceCallback)(struct Device *device);

void process_device(struct Device *device, void (*callback)(struct Device *));

void temperature_monitor(struct Device *device);
void pressure_monitor(struct Device *device);
void battery_monitor(struct Device *device);

void drift_detector(struct Device *device);

DeviceCallback callback_for_type(DeviceType type);
const char *callback_name_for_type(DeviceType type);

#endif
