#include "config.h"
#include "context.h"
#include "menu.h"
#include "sample_text.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int load_sample(TextAnalysis *ctx)
{
    TextBuffer sample;
    if (!textbuf_init(&sample, 0))
        return 0;
    if (!textbuf_append(&sample, SAMPLE_PARAGRAPH, strlen(SAMPLE_PARAGRAPH)) ||
        !analysis_replace_text(ctx, &sample, SAMPLE_TITLE, SAMPLE_DETAILS)) {
        textbuf_free(&sample);
        return 0;
    }
    return 1;
}

int main(void)
{
    TextAnalysis ctx;
    if (!analysis_init(&ctx) || !load_sample(&ctx)) {
        fprintf(stderr, "Error: could not allocate memory for the text.\n");
        analysis_free(&ctx);
        return EXIT_FAILURE;
    }

    printf("Student ID: %s\n", STUDENT_ID);
    printf("Project: Dynamic Text Mining and Linguistic Pattern Analyzer\n");
    printf("Source: %s\n", ctx.source_title);
    printf("Details: %s\n", ctx.source_details);
    printf("Custom Analysis: Lexical Diversity Score\n");
    printf("Loaded the built-in paragraph: %zu words, %zu distinct.\n", ctx.table.total_words,
           ctx.table.count);

    run_menu(&ctx);

    analysis_free(&ctx);
    printf("All memory released. Goodbye.\n");
    return EXIT_SUCCESS;
}
