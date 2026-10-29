#ifndef TEXT_STATS_H
#define TEXT_STATS_H

#include "wordstat.h"

typedef int (*WordCompare)(const void *a, const void *b);

int compare_alphabetical(const void *a, const void *b);
int compare_frequency(const void *a, const void *b);

void sort_words_by(WordTable *table, WordCompare compare);

const WordStat *most_frequent_entry(const WordTable *table);

double average_word_length(const WordTable *table);

typedef struct DiversityResult {
    double score;
    size_t hapax;
    double hapax_share;
    size_t content_total;
    size_t content_distinct;
    double content_score;
    const char *band;
} DiversityResult;

DiversityResult lexical_diversity(const WordTable *table);

int is_stop_word(const char *word);

#endif
