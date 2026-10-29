#include "operations.h"
#include "input.h"
#include "text_stats.h"

#include <stdio.h>

void count_words(TextAnalysis *ctx)
{
    printf("Total words: %zu\n", ctx->table.total_words);
    printf("Distinct words: %zu\n", ctx->table.count);
}

void longest_word(TextAnalysis *ctx)
{
    if (ctx->table.total_words == 0) {
        printf("No words in the text.\n");
        return;
    }
    printf("Longest word: \"%s\" (%zu characters)\n", ctx->table.longest,
           ctx->table.longest_length);
    if (ctx->table.longest_length > WORD_MAX_LEN)
        printf("(stored truncated to its first %d characters)\n", WORD_MAX_LEN);
}

void most_frequent(TextAnalysis *ctx)
{
    const WordStat *top = most_frequent_entry(&ctx->table);
    if (top == NULL) {
        printf("No words in the text.\n");
        return;
    }
    printf("Most frequent word: \"%s\", %d times (%.1f%% of all words)\n", top->name,
           top->count, 100.0 * top->count / (double)ctx->table.total_words);
}

void display_word_statistics(TextAnalysis *ctx)
{
    if (ctx->table.count == 0) {
        printf("No words in the text.\n");
        return;
    }
    printf("%-5s %-40s %5s\n", "#", "Word", "Count");
    int row = 1;
    for (const WordStat *p = ctx->table.items, *end = p + ctx->table.count; p < end; p++)
        printf("%-5d %-40s %5d\n", row++, p->name, p->count);
    printf("%zu distinct words, %zu words in total.\n", ctx->table.count,
           ctx->table.total_words);
}

static const struct {
    const char *label;
    WordCompare compare;
} SORT_OPTIONS[] = {
    {"Alphabetically (a to z)", compare_alphabetical},
    {"By frequency (most frequent first)", compare_frequency},
};

void sort_word_statistics(TextAnalysis *ctx)
{
    int count = (int)(sizeof SORT_OPTIONS / sizeof SORT_OPTIONS[0]);
    int choice;
    for (int i = 0; i < count; i++)
        printf("%d. %s\n", i + 1, SORT_OPTIONS[i].label);
    if (!read_int("Sort: ", 1, count, &choice))
        return;
    sort_words_by(&ctx->table, SORT_OPTIONS[choice - 1].compare);
    printf("Word statistics sorted %s.\n", SORT_OPTIONS[choice - 1].label);
    display_word_statistics(ctx);
}

void search_word(TextAnalysis *ctx)
{
    char query[128];
    if (!read_text("Word to search for: ", query, sizeof query))
        return;

    char word[WORD_MAX_LEN + 1];
    WordInfo info;
    const char *cursor = query;
    if (!next_word(&cursor, word, sizeof word, &info)) {
        printf("\"%s\" contains no letters or digits.\n", query);
        return;
    }
    const WordStat *found = table_find(&ctx->table, word);
    if (found == NULL)
        printf("\"%s\" does not appear in the text.\n", word);
    else
        printf("\"%s\" appears %d time(s) (%.1f%% of all words).\n", found->name, found->count,
               100.0 * found->count / (double)ctx->table.total_words);
}

void generate_text_summary(TextAnalysis *ctx)
{
    const WordTable *t = &ctx->table;
    printf("\n===== Text Summary =====\n");
    printf("Source: %s\n", ctx->source_title);
    printf("Details: %s\n", ctx->source_details);
    printf("Characters: %zu\n", ctx->text.length);
    printf("Total words: %zu\n", t->total_words);
    printf("Distinct words: %zu\n", t->count);
    if (t->total_words == 0) {
        printf("The text has no words.\n");
        return;
    }
    const WordStat *top = most_frequent_entry(t);
    printf("Average word length: %.2f letters\n", average_word_length(t));
    printf("Longest word: \"%s\" (%zu characters)\n", t->longest, t->longest_length);
    printf("Most frequent word: \"%s\" (%d times)\n", top->name, top->count);
    printf("Words truncated to %d characters: %zu\n", WORD_MAX_LEN, t->truncated_words);

    DiversityResult d = lexical_diversity(t);
    printf("Custom analysis - Lexical Diversity Score: %.1f / 100, %s\n", d.score, d.band);
    printf("  Content-word diversity: %.1f, words used once: %zu (%.1f%% of distinct)\n",
           d.content_score, d.hapax, d.hapax_share);
    printf("========================\n");
}
