#ifndef WORDSTAT_H
#define WORDSTAT_H

#include <stddef.h>

#include "tokenizer.h"

// One distinct word and how many times it appears (the structure the brief requires).
struct wordStat {
    char name[WORD_MAX_LEN + 1];
    int count;
};
typedef struct wordStat WordStat;

// Growable array of distinct words, plus running totals for the whole text.
typedef struct WordTable {
    WordStat *items;
    size_t count;         // distinct words stored
    size_t capacity;      // entries allocated
    size_t total_words;   // every word, repeats included
    size_t total_letters; // letters and digits across all words
    size_t truncated_words;
    char longest[WORD_MAX_LEN + 1];
    size_t longest_length; // full length of the longest word
} WordTable;

// Allocates room for initial_capacity entries with calloc. Returns 1 on success.
int table_init(WordTable *table, size_t initial_capacity);

// Frees the entries and resets every total.
void table_free(WordTable *table);

// Counts one occurrence of word: increments it if seen before (found with strcmp),
// otherwise adds it, growing the array with realloc. Returns 0 if memory runs out.
int table_add(WordTable *table, const char *word, const WordInfo *info);

// Returns the entry for word, or NULL if it is not in the text.
WordStat *table_find(const WordTable *table, const char *word);

// Empties table and fills it with every word in text. Returns 0 if memory runs out.
int table_build(WordTable *table, const char *text);

#endif
