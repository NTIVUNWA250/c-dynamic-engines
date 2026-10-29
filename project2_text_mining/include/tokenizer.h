#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stddef.h>

#define WORD_MAX_LEN 39

typedef struct WordInfo {
    size_t length;
    size_t letters;
    int truncated;
} WordInfo;

int next_word(const char **cursor, char *word, size_t size, WordInfo *info);

#endif
