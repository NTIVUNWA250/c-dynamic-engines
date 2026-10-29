#ifndef TEXT_STATS_H
#define TEXT_STATS_H

#include "wordstat.h"

// A qsort comparison strategy for word entries.
typedef int (*WordCompare)(const void *a, const void *b);

int compare_alphabetical(const void *a, const void *b); // a to z (strcmp)
int compare_frequency(const void *a, const void *b);    // most frequent first, then a to z

// Sorts the entries in place with the chosen strategy.
void sort_words_by(WordTable *table, WordCompare compare);

// Most frequent word (alphabetically first on a tie), or NULL for empty text.
const WordStat *most_frequent_entry(const WordTable *table);

// Mean letters-and-digits per word, or 0 for empty text.
double average_word_length(const WordTable *table);

// Lexical Diversity Score: how varied the vocabulary is (see docs/custom_analysis.md).
typedef struct DiversityResult {
    double score;            // 100 x distinct / total
    size_t hapax;            // words that appear exactly once
    double hapax_share;      // hapax / distinct, as a percentage
    size_t content_total;    // words that are not common stop words
    size_t content_distinct;
    double content_score;    // 100 x content_distinct / content_total
    const char *band;        // Low, Moderate or High
} DiversityResult;

DiversityResult lexical_diversity(const WordTable *table);

// 1 if word is a common English function word such as "the" or "and".
int is_stop_word(const char *word);

#endif
