#include "menu.h"
#include "input.h"
#include "log.h"

#include <stdio.h>

// Menu choice N runs MENU[N - 1].operation. The last entry (NULL) is Exit.
static const struct {
    const char *label;
    EngineOperation operation;
} MENU[] = {
    {"Register a new device", register_device},
    {"Display all devices", display_devices},
    {"Remove a device", remove_device},
    {"Generate simulated readings", generate_readings},
    {"Process readings through callbacks", process_readings},
    {"Display alerts", display_alerts},
    {"Monitoring summary", monitoring_summary},
    {"Exit", NULL},
};

#define MENU_SIZE ((int)(sizeof MENU / sizeof MENU[0]))

void run_menu(Engine *engine)
{
    for (;;) {
        printf("\n========== Menu ==========\n");
        for (int i = 0; i < MENU_SIZE; i++)
            printf("%d. %s\n", i + 1, MENU[i].label);

        int choice;
        if (!read_int("Choose an option: ", 1, MENU_SIZE, &choice)) {
            printf("\nInput ended.\n");
            return;
        }
        EngineOperation operation = MENU[choice - 1].operation;
        if (operation == NULL)
            return;
        log_msg(LOG_DEBUG, "menu", "running option %d", choice);
        operation(engine); // dispatch through the function pointer
    }
}
