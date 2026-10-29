#include "menu.h"
#include "input.h"
#include "log.h"
#include "operations.h"

#include <stdio.h>

static const struct {
    const char *label;
    AnalysisOperation operation;
} MENU[] = {
    {"Enter or paste a new paragraph", enter_paragraph},
    {"Count words", count_words},
    {"Longest word", longest_word},
    {"Most frequent word", most_frequent},
    {"Display word statistics", display_word_statistics},
    {"Sort word statistics", sort_word_statistics},
    {"Search for a word", search_word},
    {"Text summary", generate_text_summary},
    {"Custom analysis: Lexical Diversity Score", custom_analysis},
    {"Memory inspection", memory_inspection},
    {"Show the current paragraph and source", show_paragraph},
    {"Exit", NULL},
};

#define MENU_SIZE ((int)(sizeof MENU / sizeof MENU[0]))

void run_menu(TextAnalysis *ctx)
{
    for (;;) {
        printf("\nMenu\n");
        for (int i = 0; i < MENU_SIZE; i++)
            printf("%2d. %s\n", i + 1, MENU[i].label);

        int choice;
        if (!read_int("Choose an option: ", 1, MENU_SIZE, &choice)) {
            printf("\nInput ended.\n");
            return;
        }
        AnalysisOperation operation = MENU[choice - 1].operation;
        if (operation == NULL)
            return;
        log_msg(LOG_DEBUG, "menu", "running option %d", choice);
        operation(ctx);
    }
}
