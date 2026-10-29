#include "menu.h"
#include "actions.h"
#include "input.h"
#include "log.h"

#include <stdio.h>

// Menu choice N runs MENU[N - 1].action. The last entry (NULL action) is Exit.
static const struct {
    const char *label;
    MenuAction action;
} MENU[] = {
    {"Add a new student", add_student},
    {"Display all students", display_students},
    {"Sort students", sort_students},
    {"Calculate average grade", calculate_average},
    {"Find top-performing student", find_top_student},
    {"Search for a student", search_student},
    {"Remove a student", remove_student},
    {"Run custom analysis (Academic Risk Index)", custom_analysis},
    {"Display summary report", generate_summary_report},
    {"Exit", NULL},
};

#define MENU_SIZE ((int)(sizeof MENU / sizeof MENU[0]))

void run_menu(StudentList *list)
{
    for (;;) {
        printf("\n========== Menu ==========\n");
        for (int i = 0; i < MENU_SIZE; i++)
            printf("%2d. %s\n", i + 1, MENU[i].label);

        int choice;
        if (!read_int("Choose an option: ", 1, MENU_SIZE, &choice)) {
            printf("\nInput ended.\n");
            return;
        }
        MenuAction action = MENU[choice - 1].action;
        if (action == NULL)
            return;
        log_msg(LOG_DEBUG, "menu", "running option %d", choice);
        action(list); // dispatch through the function pointer
    }
}
