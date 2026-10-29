#include "test.h"
#include "tokenizer.h"

#include <string.h>

// Pulls every word out of text and joins them with '|' for easy comparison.
static void words_of(const char *text, char *joined, size_t size)
{
    char word[WORD_MAX_LEN + 1];
    WordInfo info;
    const char *cursor = text;
    joined[0] = '\0';
    while (next_word(&cursor, word, sizeof word, &info)) {
        if (joined[0] != '\0')
            strncat(joined, "|", size - strlen(joined) - 1);
        strncat(joined, word, size - strlen(joined) - 1);
    }
}

// Punctuation, apostrophes, hyphens, case, whitespace and long words.
int main(void)
{
    char out[512];

    words_of("Call me Ishmael.", out, sizeof out);
    CHECK(strcmp(out, "call|me|ishmael") == 0);

    // Case is folded, repeated whitespace and newlines are ignored.
    words_of("  The   THE\tthe\n\nThe  ", out, sizeof out);
    CHECK(strcmp(out, "the|the|the|the") == 0);

    // Apostrophes inside a word stay; quotes around a word do not.
    words_of("people's hats; don't 'quoted' dogs'", out, sizeof out);
    CHECK(strcmp(out, "people's|hats|don't|quoted|dogs") == 0);

    // A single hyphen joins; a double hyphen (dash) separates.
    words_of("well-known ago--never -dash- end-", out, sizeof out);
    CHECK(strcmp(out, "well-known|ago|never|dash|end") == 0);

    // Digits count as word characters.
    words_of("In 1851, chapter 1.", out, sizeof out);
    CHECK(strcmp(out, "in|1851|chapter|1") == 0);

    // Only punctuation, or empty: no words.
    words_of("... !!! --", out, sizeof out);
    CHECK(strcmp(out, "") == 0);
    words_of("", out, sizeof out);
    CHECK(strcmp(out, "") == 0);

    // Non-ASCII bytes (here UTF-8 curly quotes) act as separators.
    words_of("\xe2\x80\x9cHello\xe2\x80\x9d", out, sizeof out);
    CHECK(strcmp(out, "hello") == 0);

    // Lengths: full length counts ' and -, letters do not.
    char word[WORD_MAX_LEN + 1];
    WordInfo info;
    const char *cursor = "people's";
    CHECK(next_word(&cursor, word, sizeof word, &info));
    CHECK(info.length == 8 && info.letters == 7 && !info.truncated);

    // A 45-letter word is stored as its first 39 letters, but its length is kept.
    const char *longword = "pneumonoultramicroscopicsilicovolcanoconiosis!";
    cursor = longword;
    CHECK(next_word(&cursor, word, sizeof word, &info));
    CHECK(strlen(word) == WORD_MAX_LEN);
    CHECK(strncmp(word, longword, WORD_MAX_LEN) == 0);
    CHECK(info.length == 45 && info.truncated);
    CHECK(*cursor == '!'); // the cursor moved past the whole word

    return TEST_DONE();
}
