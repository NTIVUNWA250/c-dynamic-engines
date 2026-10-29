#include "test.h"
#include "wordstat.h"

#include <string.h>

// Counting repeats, totals, longest word, growth and rebuilding.
int main(void)
{
    WordTable table;
    CHECK(table_init(&table, 0));
    CHECK(table.capacity >= 16 && table.count == 0);

    CHECK(table_build(&table, "The cat saw the other cat. THE END!"));
    CHECK(table.total_words == 8);
    CHECK(table.count == 5); // the, cat, saw, other, end
    CHECK(table_find(&table, "the") != NULL && table_find(&table, "the")->count == 3);
    CHECK(table_find(&table, "cat")->count == 2);
    CHECK(table_find(&table, "dog") == NULL);
    CHECK(strcmp(table.longest, "other") == 0 && table.longest_length == 5);
    CHECK(table.total_letters == 3 + 3 + 3 + 3 + 5 + 3 + 3 + 3);

    // Sum of all counts equals the total number of words.
    size_t sum = 0;
    for (size_t i = 0; i < table.count; i++)
        sum += (size_t)table.items[i].count;
    CHECK(sum == table.total_words);

    // Rebuilding with new text replaces the old results.
    CHECK(table_build(&table, "Alpha beta"));
    CHECK(table.total_words == 2 && table.count == 2);
    CHECK(table_find(&table, "cat") == NULL);

    // Many distinct words make the array grow past its starting size.
    char text[4000] = "";
    for (int i = 0; i < 300; i++) {
        char word[16];
        int n = i;
        int k = 0;
        do { // spell i with letters only: 0 -> "a", 26 -> "ab"...
            word[k++] = (char)('a' + n % 26);
            n /= 26;
        } while (n > 0 && k < 14);
        word[k++] = ' ';
        word[k] = '\0';
        strcat(text, word);
    }
    CHECK(table_build(&table, text));
    CHECK(table.count == 300 && table.capacity >= 300);

    // A long word is stored truncated and counted.
    CHECK(table_build(&table, "pneumonoultramicroscopicsilicovolcanoconiosis is long"));
    CHECK(table.truncated_words == 1);
    CHECK(table.longest_length == 45);
    CHECK(strlen(table.longest) == WORD_MAX_LEN);

    // Empty text gives an empty table.
    CHECK(table_build(&table, "   ...  "));
    CHECK(table.total_words == 0 && table.count == 0 && table.longest_length == 0);

    table_free(&table);
    CHECK(table.items == NULL);
    return TEST_DONE();
}
