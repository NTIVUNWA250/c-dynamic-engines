#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stddef.h>

// Word rules (see docs/text_rules.md):
//  - A word is a run of ASCII letters and digits.
//  - An apostrophe or hyphen stays inside a word only when it sits between two
//    letters or digits: "people's", "well-known". "--" and a trailing ' split words.
//  - All other characters (punctuation, spaces, newlines, non-ASCII bytes) separate words.
//  - Words are stored in lowercase, so "The" and "the" are the same word.
//  - A word longer than WORD_MAX_LEN is truncated to its first WORD_MAX_LEN
//    characters for storage; its full length is still reported.
#define WORD_MAX_LEN 39 // fits struct wordStat.name[40] with the '\0'

// Details about the word just extracted.
typedef struct WordInfo {
    size_t length;  // full length in the text, including ' and -
    size_t letters; // letters and digits only (used for average word length)
    int truncated;  // 1 if length > WORD_MAX_LEN
} WordInfo;

// Finds the next word starting at *cursor, writes it lowercased into word (size >= 2)
// and moves *cursor past it. Returns 1 if a word was found, 0 at the end of the text.
int next_word(const char **cursor, char *word, size_t size, WordInfo *info);

#endif
