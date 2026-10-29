#include "input.h"
#include "log.h"
#include "operations.h"
#include "textio.h"

#include <stdio.h>

void enter_paragraph(TextAnalysis *ctx)
{
    char title[SOURCE_LEN];
    char details[SOURCE_LEN];
    if (!read_text("Source title: ", title, sizeof title) ||
        !read_text("Author and publication details: ", details, sizeof details))
        return;

    TextBuffer fresh;
    if (!textbuf_init(&fresh, 0)) {
        printf("Not enough memory to read the text.\n");
        return;
    }
    printf("Paste or type the paragraph. Press Enter on an empty line to finish.\n");
    int status = read_paragraph(&fresh);
    if (status <= 0) {
        printf(status == 0 ? "Empty text, keeping the previous paragraph.\n"
                           : "Ran out of memory, keeping the previous paragraph.\n");
        textbuf_free(&fresh);
        return;
    }
    if (!analysis_replace_text(ctx, &fresh, title, details)) {
        printf("Ran out of memory analysing the text, keeping the previous paragraph.\n");
        textbuf_free(&fresh);
        return;
    }
    printf("Loaded \"%s\": %zu words, %zu distinct.\n", ctx->source_title,
           ctx->table.total_words, ctx->table.count);
    if (ctx->table.total_words < MIN_RECOMMENDED_WORDS)
        printf("Note: this text has fewer than %d words.\n", MIN_RECOMMENDED_WORDS);
}

void show_paragraph(TextAnalysis *ctx)
{
    printf("\nSource: %s\nDetails: %s\n\n%s\n", ctx->source_title, ctx->source_details,
           ctx->text.data);
}
