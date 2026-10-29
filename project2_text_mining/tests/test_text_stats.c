#include "test.h"
#include "text_stats.h"

#include <string.h>

// Sorting, most frequent word, average length and the diversity score.
int main(void)
{
    WordTable table;
    table_init(&table, 0);

    // Empty text: no crashes, zero results.
    table_build(&table, "");
    CHECK(most_frequent_entry(&table) == NULL);
    CHECK(average_word_length(&table) == 0.0);
    CHECK(lexical_diversity(&table).score == 0.0);

    // 10 words, 6 distinct: the x3, cat x2, sat x2, on, mat, dog.
    table_build(&table, "The cat sat. The cat sat on the mat, dog!");
    CHECK(table.total_words == 10 && table.count == 6);

    const WordStat *top = most_frequent_entry(&table);
    CHECK(top != NULL && strcmp(top->name, "the") == 0 && top->count == 3);
    CHECK_NEAR(average_word_length(&table), 29.0 / 10.0, 1e-9);

    sort_words_by(&table, compare_alphabetical);
    CHECK(strcmp(table.items[0].name, "cat") == 0);
    CHECK(strcmp(table.items[5].name, "the") == 0);

    sort_words_by(&table, compare_frequency);
    CHECK(strcmp(table.items[0].name, "the") == 0);
    CHECK(strcmp(table.items[1].name, "cat") == 0); // cat and sat tie at 2: a to z
    CHECK(strcmp(table.items[2].name, "sat") == 0);

    // Ties for most frequent pick the alphabetically first word.
    table_build(&table, "zebra apple zebra apple");
    CHECK(strcmp(most_frequent_entry(&table)->name, "apple") == 0);

    // Diversity: 10 words, 6 distinct -> 60. Hapax: on, mat, dog -> 3 of 6.
    table_build(&table, "The cat sat. The cat sat on the mat, dog!");
    DiversityResult d = lexical_diversity(&table);
    CHECK_NEAR(d.score, 60.0, 1e-9);
    CHECK(d.hapax == 3);
    CHECK_NEAR(d.hapax_share, 50.0, 1e-9);
    // Stop words removed (the, on): cat x2, sat x2, mat, dog -> 4 distinct of 6.
    CHECK(d.content_distinct == 4 && d.content_total == 6);
    CHECK_NEAR(d.content_score, 400.0 / 6.0, 1e-9);
    CHECK(strcmp(d.band, "High (varied)") == 0);

    table_build(&table, "go go go go go stop");
    CHECK(strcmp(lexical_diversity(&table).band, "Low (repetitive)") == 0);

    CHECK(is_stop_word("the") && is_stop_word("and") && !is_stop_word("whale"));

    table_free(&table);
    return TEST_DONE();
}
