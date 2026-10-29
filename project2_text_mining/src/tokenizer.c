#include "tokenizer.h"

#include <ctype.h>

static int is_word_char(char c)
{
    return isalnum((unsigned char)c) != 0;
}

static int is_joiner(char c)
{
    return c == '\'' || c == '-';
}

int next_word(const char **cursor, char *word, size_t size, WordInfo *info)
{
    const char *p = *cursor;

    while (*p != '\0' && !is_word_char(*p))
        p++;
    if (*p == '\0') {
        *cursor = p;
        return 0;
    }

    char *out = word;
    char *last = word + size - 1;
    size_t length = 0;
    size_t letters = 0;

    while (*p != '\0') {
        if (is_word_char(*p)) {
            letters++;
        } else if (!(is_joiner(*p) && is_word_char(p[1]))) {
            break;
        }
        if (out < last)
            *out++ = (char)tolower((unsigned char)*p);
        length++;
        p++;
    }
    *out = '\0';
    *cursor = p;

    info->length = length;
    info->letters = letters;
    info->truncated = length > size - 1;
    return 1;
}
