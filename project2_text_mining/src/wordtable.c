#include "wordstat.h"
#include "log.h"

#include <stdlib.h>
#include <string.h>

#define MIN_CAPACITY 16

int table_init(WordTable *table, size_t initial_capacity)
{
    memset(table, 0, sizeof *table);
    if (initial_capacity < MIN_CAPACITY)
        initial_capacity = MIN_CAPACITY;
    table->items = calloc(initial_capacity, sizeof *table->items);
    if (table->items == NULL) {
        log_msg(LOG_ERROR, "words", "could not allocate %zu entries", initial_capacity);
        return 0;
    }
    table->capacity = initial_capacity;
    return 1;
}

void table_free(WordTable *table)
{
    free(table->items);
    memset(table, 0, sizeof *table);
}

WordStat *table_find(const WordTable *table, const char *word)
{
    for (WordStat *p = table->items, *end = table->items + table->count; p < end; p++)
        if (strcmp(p->name, word) == 0)
            return p;
    return NULL;
}

static void record_totals(WordTable *table, const char *word, const WordInfo *info)
{
    table->total_words++;
    table->total_letters += info->letters;
    if (info->truncated)
        table->truncated_words++;
    if (info->length > table->longest_length) {
        table->longest_length = info->length;
        strcpy(table->longest, word);
    }
}

int table_add(WordTable *table, const char *word, const WordInfo *info)
{
    if (strlen(word) > WORD_MAX_LEN)
        return 0;

    WordStat *existing = table_find(table, word);
    if (existing != NULL) {
        existing->count++;
        record_totals(table, word, info);
        return 1;
    }

    if (table->count == table->capacity) {
        size_t new_capacity = table->capacity < MIN_CAPACITY ? MIN_CAPACITY : table->capacity * 2;
        WordStat *grown = realloc(table->items, new_capacity * sizeof *grown);
        if (grown == NULL) {
            log_msg(LOG_ERROR, "words", "realloc to %zu entries failed", new_capacity);
            return 0;
        }
        log_msg(LOG_INFO, "words", "entries %zu -> %zu", table->capacity, new_capacity);
        table->items = grown;
        table->capacity = new_capacity;
    }

    WordStat *slot = table->items + table->count;
    strcpy(slot->name, word);
    slot->count = 1;
    table->count++;
    record_totals(table, word, info);
    return 1;
}

int table_build(WordTable *table, const char *text)
{
    WordStat *items = table->items;
    size_t capacity = table->capacity;
    memset(table, 0, sizeof *table);
    table->items = items;
    table->capacity = capacity;

    char word[WORD_MAX_LEN + 1];
    WordInfo info;
    const char *cursor = text;
    while (next_word(&cursor, word, sizeof word, &info))
        if (!table_add(table, word, &info))
            return 0;

    log_msg(LOG_INFO, "words", "%zu words, %zu distinct", table->total_words, table->count);
    return 1;
}
