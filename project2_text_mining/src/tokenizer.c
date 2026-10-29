#include "tokenizer.h"

#include <ctype.h>

// Letters and digits make up words.
static int is_word_char(char c)
{
    return isalnum((unsigned char)c) != 0;
}

// Apostrophes and hyphens may join two parts of one word.
static int is_joiner(char c)
{
    return c == '\'' || c == '-';
}

int next_word(const char **cursor, char *word, size_t size, WordInfo *info)
{
    const char *p = *cursor;

    // Skip separators until the first letter or digit.
    while (*p != '\0' && !is_word_char(*p))
        p++;
    if (*p == '\0') {
        *cursor = p;
        return 0;
    }

    char *out = word;
    char *last = word + size - 1; // keep the final byte for '\0'
    size_t length = 0;
    size_t letters = 0;

    // Walk the word one character at a time with the pointer p.
    while (*p != '\0') {
        if (is_word_char(*p)) {
            letters++;
        } else if (!(is_joiner(*p) && is_word_char(p[1]))) {
            break; // any other character, or a joiner not followed by a letter, ends the word
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
