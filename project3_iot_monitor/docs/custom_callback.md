# Custom Callback: Sensor Drift Detector (`drift_detector`)

## Purpose

The type callbacks only ask "is this reading inside the safe range?". A reading can
pass that check and still be suspicious. For example, a greenhouse that jumps from
20 C to 41 C between two readings is still "in range", but something is wrong: a
vent stuck shut, a heater left on, or a faulty sensor. The drift detector
catches these sudden changes, whether or not the new value is in range.

## Operating conditions

| Type | Drift limit per reading |
|---|---|
| Temperature | 10.00 C |
| Pressure | 60.00 hPa |
| Battery | 0.35 V |

The limits are in `src/device.c` (`drift_limit`).

## Algorithm

It runs on every processed reading, after the type callback, through
`process_device(device, drift_detector)`.

```
value = device_value(device)          // reads the union member for the device's type
if device has a previous reading:
    change = value - previous
    if |change| > drift_limit[type]:
        status = ALERT
        log an alert: "sudden rise/drop of |change| <unit> since the last reading",
                      expected range = previous +/- drift_limit, callback = drift_detector
        count it (total and per type)
    track the largest |change| / drift_limit seen so far
previous = value                      // the first reading only sets this baseline
```

## Results it produces

- An alert per drift event, listed in "Display alerts" with the expected window
  (`previous - limit` to `previous + limit`).
- An extra `DRIFT:` line under the reading in the processing table.
- In the summary:
  - drift checks;
  - drift alerts per device type;
  - the largest jump, as a percentage of its limit (e.g. 190% means almost twice
    the allowed change).

## Simulated readings

To make drift meaningful, readings are a **random walk**, not independent random
numbers (`src/simulation.c`):

| Reading | How it is generated |
|---|---|
| First | Random, inside the normal range |
| Usually (85%) | Last value + a random step of up to +/- `step`, pulled 15% of the way back to the middle of the normal range |
| Sometimes (15%) | A spike of 1.2 to 2.5 drift limits, up or down |

Every value is clamped to the documented simulation range:

| Type | Simulation range | Normal range | Step |
|---|---|---|---|
| Temperature | 5 to 60 C | 15 to 45 C | 6 C |
| Pressure | 850 to 1150 hPa | 950 to 1050 hPa | 35 hPa |
| Battery | 2.9 to 4.4 V | 3.3 to 4.2 V | 0.2 V |

`rand()` is seeded with `srand(time(NULL))`, or with `--seed N` to repeat a run
exactly.
