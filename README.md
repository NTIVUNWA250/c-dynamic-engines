# C Dynamic Engines

Three C11 programs that practise structures, unions, dynamic memory, pointers and
function pointers.

| Folder | Project |
|---|---|
| `project1_academic_risk/` | Dynamic Academic Performance and Risk Analysis Engine |
| `project2_text_mining/` | Dynamic Text Mining and Linguistic Pattern Analyzer |
| `project3_iot_monitor/` | Dynamic IoT Event Monitoring and Callback Engine |
| `common/` | Shared helpers: safe input, logging, test macros |

Each project has its own README with build steps, design notes and its custom
analysis.

## Requirements

- gcc (C11) and make, on Linux or macOS. On Windows, use WSL.

## Run all tests

```
make test
```

## Build and run one project

```
cd project1_academic_risk
make
./bin/academic_engine
```

## Logs

Logs go to stderr and are hidden by default. To see them:

```
APP_LOG=info ./bin/academic_engine
```
