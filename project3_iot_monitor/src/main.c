#include "config.h"
#include "engine.h"
#include "input.h"
#include "log.h"
#include "menu.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static unsigned seed_from_args(int argc, char **argv)
{
    int seed;
    if (argc == 3 && strcmp(argv[1], "--seed") == 0 && parse_int(argv[2], 0, INT_MAX, &seed)) {
        printf("Random seed: %d (fixed)\n", seed);
        return (unsigned)seed;
    }
    if (argc > 1)
        printf("Usage: %s [--seed N]. Ignoring the arguments.\n", argv[0]);
    return (unsigned)time(NULL);
}

int main(int argc, char **argv)
{
    printf("Student ID: %s\n", STUDENT_ID);
    printf("Project: Dynamic IoT Event Monitoring and Callback Engine\n");
    printf("Custom Callback: Sensor Drift Detector\n");
    srand(seed_from_args(argc, argv));

    Engine engine;
    if (!engine_init(&engine)) {
        fprintf(stderr, "Error: could not allocate memory for the devices.\n");
        return EXIT_FAILURE;
    }
    printf("Registered %zu starting devices.\n", engine.devices.count);

    run_menu(&engine);

    engine_free(&engine);
    printf("All memory released. Goodbye.\n");
    return EXIT_SUCCESS;
}
