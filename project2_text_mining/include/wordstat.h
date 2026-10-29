#ifndef WORDSTAT_H
#define WORDSTAT_H

#include <stddef.h>

#include "tokenizer.h"

struct wordStat {
    char name[WORD_MAX_LEN + 1];
    int count;
};
typedef struct wordStat WordStat;

typedef struct WordTable {
    WordStat *items;
    size_t count;
    size_t capacity;
    size_t total_words;
    size_t total_letters;
    size_t truncated_words;
    char longest[WORD_MAX_LEN + 1];
    size_t longest_length;
} WordTable;

int table_init(WordTable *table, size_t initial_capacity);

void table_free(WordTable *table);

int table_add(WordTable *table, const char *word, const WordInfo *info);

WordStat *table_find(const WordTable *table, const char *word);

int table_build(WordTable *table, const char *text);

#endif
