#include "sample_text.h"
#include "test.h"
#include "text_stats.h"
#include "textbuf.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

#define COPIES 1000
#define BUDGET_SECONDS 1.0

static double seconds_since(clock_t start)
{
    return (double)(clock() - start) / CLOCKS_PER_SEC;
}

#define TIMED(label, code)                                     \
    do {                                                       \
        clock_t start_ = clock();                              \
        code;                                                  \
        double took_ = seconds_since(start_);                  \
        printf("%-32s %.3fs\n", label, took_);                 \
        CHECK(took_ < BUDGET_SECONDS);                         \
    } while (0)

int main(void)
{
    TextBuffer text;
    CHECK(textbuf_init(&text, 0));
    size_t paragraph = strlen(SAMPLE_PARAGRAPH);

    TIMED("grow text to ~1.1 MB", for (int i = 0; i < COPIES; i++) {
        textbuf_append(&text, SAMPLE_PARAGRAPH, paragraph);
        textbuf_append(&text, " ", 1);
    });
    CHECK(text.length == COPIES * (paragraph + 1));

    WordTable table;
    CHECK(table_init(&table, 0));
    TIMED("build table (~200k words)", CHECK(table_build(&table, text.data)));
    CHECK(table.total_words == 201u * COPIES);
    CHECK(table.count == 134);

    TIMED("sort by frequency", sort_words_by(&table, compare_frequency));
    TIMED("sort alphabetically", sort_words_by(&table, compare_alphabetical));
    TIMED("lexical diversity", lexical_diversity(&table));

    table_free(&table);
    textbuf_free(&text);
    return TEST_DONE();
}
