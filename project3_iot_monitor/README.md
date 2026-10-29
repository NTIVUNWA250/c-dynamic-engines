# Project 3: Dynamic IoT Event Monitoring and Callback Engine

A menu-driven C program that simulates temperature, pressure and battery devices,
queues their readings, and processes each one through callback functions chosen by
function pointer. A custom **Sensor Drift Detector** callback flags sudden jumps.

## Build and run

```
make                         # builds bin/iot_monitor
./bin/iot_monitor            # random readings every run
./bin/iot_monitor --seed 42  # the same readings every run
```

Make a sample output from the scripted session:

```
./bin/iot_monitor --seed 42 < samples/demo_input.txt > samples/sample_output.txt
```

## Test

```
make test        # unit + integration + performance
```

## Menu

| # | Operation | Function |
|---|---|---|
| 1 | Register a device | `register_device()` |
| 2 | Display devices | `display_devices()` |
| 3 | Remove a device | `remove_device()` |
| 4 | Generate simulated readings | `generate_readings()` |
| 5 | Process readings through callbacks | `process_readings()` |
| 6 | Display alerts | `display_alerts()` |
| 7 | Monitoring summary | `monitoring_summary()` |
| 8 | Exit | frees memory and quits |

The setting is a small farm. Six devices are registered at start, two of each type:
greenhouse and poultry house temperature, irrigation main and drip line pressure, and
solar pump and weather station batteries. One round of simulation
gives every device a reading, so 2 rounds = 12 readings.

## Where each requirement lives

| Requirement | File |
|---|---|
| `struct Device` with a `union` of readings and a status | `include/device.h` |
| Correct union member per type (never read another) | `src/device.c` (`device_value`, `format_reading`), callbacks, `src/simulation.c` packers |
| Device array: `calloc`, grows/shrinks with `realloc`, no duplicate IDs | `src/registry.c` |
| `process_device(struct Device *, void (*)(struct Device *))` | `src/callbacks.c` |
| `temperature_monitor`, `pressure_monitor`, `battery_monitor` | `src/callbacks.c` |
| Callback chosen by a table indexed by type (no big if/else) | `src/callbacks.c` (`TYPE_CALLBACKS`) |
| Custom callback, called through a function pointer | `src/drift.c`, `docs/custom_callback.md` |
| `rand()` seeded with `srand()`, documented ranges | `src/simulation.c`, `src/main.c`, `src/device.c` |
| Pointer traversal of the device array | `src/registry.c`, `src/simulation.c`, `src/device_ops.c` |
| Alerts with value, unit, expected range, reason and callback | `src/monitor.c`, `src/monitor_ops.c` |
| Summary report | `src/monitor_ops.c` |
| Function-pointer menu table (`EngineOperation`) | `src/menu.c` |
| Allocation failures handled | `tests/test_alloc_failure.c` |

## Safe removal

Queued readings store the device **ID**, not a pointer. If a device is removed (or
the array moves after `realloc`) before its reading is processed, the reading is
skipped with a message instead of touching freed memory. The sample session shows
this for device 202.
