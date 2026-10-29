#include "context.h"
#include "test.h"
#include "test_alloc.h"

#include <string.h>

// When malloc/calloc/realloc fail, nothing crashes and old results are kept.
int main(void)
{
    // Text buffer: init fails cleanly, a failed append keeps the old text.
    TextBuffer buf;
    test_alloc_fail_after(0);
    CHECK(!textbuf_init(&buf, 0));
    CHECK(buf.data == NULL);
    CHECK(textbuf_init(&buf, 0));
    CHECK(textbuf_append(&buf, "keep me", 7));
    char big[200];
    memset(big, 'x', sizeof big);
    test_alloc_fail_after(0);
    CHECK(!textbuf_append(&buf, big, sizeof big));
    CHECK(strcmp(buf.data, "keep me") == 0);
    textbuf_free(&buf);

    // Word table: a failed realloc while adding new words is reported.
    WordTable table;
    CHECK(table_init(&table, 0));
    char text[400] = "";
    for (int i = 0; i < 40; i++) {
        char word[4] = {(char)('a' + i % 26), (char)('a' + i / 26), ' ', '\0'};
        strcat(text, word);
    }
    test_alloc_fail_after(0);
    CHECK(!table_build(&table, text));
    table_free(&table);

    // Context: replacing the text fails, and the previous text and results survive.
    TextAnalysis ctx;
    CHECK(analysis_init(&ctx));
    TextBuffer first;
    textbuf_init(&first, 0);
    textbuf_append(&first, "one two two", 11);
    CHECK(analysis_replace_text(&ctx, &first, "First", "test"));

    TextBuffer second;
    textbuf_init(&second, 0);
    textbuf_append(&second, "three", 5);
    test_alloc_fail_after(0); // the new word table cannot be allocated
    CHECK(!analysis_replace_text(&ctx, &second, "Second", "test"));
    CHECK(strcmp(ctx.source_title, "First") == 0);
    CHECK(ctx.table.total_words == 3);
    textbuf_free(&second);

    analysis_free(&ctx);
    return TEST_DONE();
}
