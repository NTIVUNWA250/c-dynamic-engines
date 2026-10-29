#include "operations.h"
#include "text_stats.h"

#include <stdio.h>

void custom_analysis(TextAnalysis *ctx)
{
    const WordTable *t = &ctx->table;
    printf("\n-- Custom analysis: Lexical Diversity Score --\n");
    printf("Score = 100 x distinct words / total words\n");
    if (t->total_words == 0) {
        printf("No words to analyse.\n");
        return;
    }

    DiversityResult d = lexical_diversity(t);
    printf("Distinct / total: %zu / %zu\n", t->count, t->total_words);
    printf("Lexical Diversity Score: %.1f / 100 -> %s\n", d.score, d.band);
    printf("Words used exactly once (hapax): %zu = %.1f%% of distinct words\n", d.hapax,
           d.hapax_share);
    printf("Content words (stop words such as \"the\", \"and\" removed): %zu distinct / %zu "
           "total\n",
           d.content_distinct, d.content_total);
    printf("Content-word diversity: %.1f / 100\n", d.content_score);
    printf("Bands: below 40 Low (repetitive), 40 to 59.9 Moderate, 60+ High (varied).\n");
    if (d.content_score > d.score)
        printf("Reading: the repetition comes mostly from small function words; the "
               "meaningful vocabulary is more varied than the raw score suggests.\n");
    else
        printf("Reading: meaningful words repeat at least as often as function words, "
               "so the text keeps returning to the same ideas.\n");
}

void memory_inspection(TextAnalysis *ctx)
{
    const WordTable *t = &ctx->table;
    printf("\nMemory Inspection\n--------------------------\n");
    printf("Text buffer address:       %p\n", (void *)ctx->text.data);
    printf("  heap block holding the paragraph: %zu chars used of %zu bytes reserved\n",
           ctx->text.length, ctx->text.capacity);
    printf("Word statistics array:     %p\n", (void *)t->items);
    printf("  heap block of struct wordStat: %zu used of %zu reserved, %zu bytes each\n",
           t->count, t->capacity, sizeof(WordStat));

    const WordStat *selected = most_frequent_entry(t);
    if (selected == NULL) {
        printf("No word entries to inspect.\n");
        return;
    }
    // Pointer subtraction gives the entry's position in the array.
    ptrdiff_t index = selected - t->items;
    printf("Selected word-stat entry:  %p\n", (void *)selected);
    printf("  entry [%td] \"%s\" (the most frequent word) = array + %td x %zu bytes\n", index,
           selected->name, index, sizeof(WordStat));
    printf("  its name field: %p, its count field: %p\n", (void *)selected->name,
           (void *)&selected->count);
    printf("Context structure (stack): %p\n", (void *)ctx);
    printf("Addresses change from run to run; heap blocks may move after realloc.\n");
}
