#include "text_stats.h"
#include "log.h"

#include <stdlib.h>
#include <string.h>

int compare_alphabetical(const void *a, const void *b)
{
    const WordStat *x = a;
    const WordStat *y = b;
    return strcmp(x->name, y->name);
}

int compare_frequency(const void *a, const void *b)
{
    const WordStat *x = a;
    const WordStat *y = b;
    if (x->count != y->count)
        return x->count < y->count ? 1 : -1;
    return strcmp(x->name, y->name);
}

void sort_words_by(WordTable *table, WordCompare compare)
{
    if (table->count > 1)
        qsort(table->items, table->count, sizeof *table->items, compare);
    log_msg(LOG_INFO, "stats", "sorted %zu words", table->count);
}

const WordStat *most_frequent_entry(const WordTable *table)
{
    const WordStat *best = NULL;
    for (const WordStat *p = table->items, *end = p + table->count; p < end; p++)
        if (best == NULL || compare_frequency(p, best) < 0)
            best = p;
    return best;
}

double average_word_length(const WordTable *table)
{
    if (table->total_words == 0)
        return 0.0;
    return (double)table->total_letters / (double)table->total_words;
}

// Common English function words, which carry little meaning on their own.
static const char *STOP_WORDS[] = {
    "a",    "about", "all",  "an",   "and",   "are",  "as",    "at",   "be",   "but",
    "by",   "do",    "for",  "from", "had",   "has",  "have",  "he",   "her",  "his",
    "i",    "if",    "in",   "is",   "it",    "its",  "me",    "my",   "no",   "not",
    "of",   "on",    "or",   "she",  "so",    "that", "the",   "their", "them", "they",
    "this", "to",    "was",  "we",   "were",  "with", "you",   "your",
};

int is_stop_word(const char *word)
{
    size_t n = sizeof STOP_WORDS / sizeof STOP_WORDS[0];
    for (const char **p = STOP_WORDS; p < STOP_WORDS + n; p++)
        if (strcmp(*p, word) == 0)
            return 1;
    return 0;
}

DiversityResult lexical_diversity(const WordTable *table)
{
    DiversityResult result = {0};
    result.band = "n/a";
    if (table->total_words == 0)
        return result;

    for (const WordStat *p = table->items, *end = p + table->count; p < end; p++) {
        if (p->count == 1)
            result.hapax++;
        if (!is_stop_word(p->name)) {
            result.content_distinct++;
            result.content_total += (size_t)p->count;
        }
    }

    result.score = 100.0 * (double)table->count / (double)table->total_words;
    result.hapax_share = 100.0 * (double)result.hapax / (double)table->count;
    if (result.content_total > 0)
        result.content_score =
            100.0 * (double)result.content_distinct / (double)result.content_total;

    result.band = result.score < 40.0 ? "Low (repetitive)"
                  : result.score < 60.0 ? "Moderate"
                                        : "High (varied)";
    log_msg(LOG_INFO, "stats", "lexical diversity %.1f", result.score);
    return result;
}
