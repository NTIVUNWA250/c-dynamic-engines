#include "test.h"
#include "tokenizer.h"

#include <string.h>

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

int main(void)
{
    char out[512];

    words_of("Call me Ishmael.", out, sizeof out);
    CHECK(strcmp(out, "call|me|ishmael") == 0);

    words_of("  The   THE\tthe\n\nThe  ", out, sizeof out);
    CHECK(strcmp(out, "the|the|the|the") == 0);

    words_of("people's hats; don't 'quoted' dogs'", out, sizeof out);
    CHECK(strcmp(out, "people's|hats|don't|quoted|dogs") == 0);

    words_of("well-known ago--never -dash- end-", out, sizeof out);
    CHECK(strcmp(out, "well-known|ago|never|dash|end") == 0);

    words_of("In 1851, chapter 1.", out, sizeof out);
    CHECK(strcmp(out, "in|1851|chapter|1") == 0);

    words_of("... !!! --", out, sizeof out);
    CHECK(strcmp(out, "") == 0);
    words_of("", out, sizeof out);
    CHECK(strcmp(out, "") == 0);

    words_of("\xe2\x80\x9cHello\xe2\x80\x9d", out, sizeof out);
    CHECK(strcmp(out, "hello") == 0);

    char word[WORD_MAX_LEN + 1];
    WordInfo info;
    const char *cursor = "people's";
    CHECK(next_word(&cursor, word, sizeof word, &info));
    CHECK(info.length == 8 && info.letters == 7 && !info.truncated);

    const char *longword = "pneumonoultramicroscopicsilicovolcanoconiosis!";
    cursor = longword;
    CHECK(next_word(&cursor, word, sizeof word, &info));
    CHECK(strlen(word) == WORD_MAX_LEN);
    CHECK(strncmp(word, longword, WORD_MAX_LEN) == 0);
    CHECK(info.length == 45 && info.truncated);
    CHECK(*cursor == '!');

    return TEST_DONE();
}
