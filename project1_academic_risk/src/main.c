#include "config.h"
#include "dataset.h"
#include "log.h"
#include "menu.h"
#include "records.h"

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    printf("Student ID: %s\n", STUDENT_ID);
    printf("Project: Dynamic Academic Performance and Risk Analysis Engine\n");
    printf("Custom Analysis: Academic Risk Index\n");

    StudentList list;
    if (!dataset_load(&list)) {
        fprintf(stderr, "Error: could not allocate memory for the starting dataset.\n");
        return EXIT_FAILURE;
    }
    printf("Loaded %zu starting student records.\n", list.count);

    run_menu(&list);

    list_free(&list);
    printf("All memory released. Goodbye.\n");
    return EXIT_SUCCESS;
}
