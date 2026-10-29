#include "context.h"
#include "log.h"

#include <stdio.h>
#include <string.h>

int analysis_init(TextAnalysis *ctx)
{
    memset(ctx, 0, sizeof *ctx);
    if (!textbuf_init(&ctx->text, 0))
        return 0;
    if (!table_init(&ctx->table, 0)) {
        textbuf_free(&ctx->text);
        return 0;
    }
    return 1;
}

int analysis_replace_text(TextAnalysis *ctx, TextBuffer *new_text, const char *title,
                          const char *details)
{
    // Build the new word table separately, so a failure leaves the old results intact.
    WordTable fresh;
    if (!table_init(&fresh, 0))
        return 0;
    if (!table_build(&fresh, new_text->data)) {
        table_free(&fresh);
        return 0;
    }

    textbuf_free(&ctx->text);
    table_free(&ctx->table);
    ctx->text = *new_text;
    ctx->table = fresh;
    // The context now owns the text; empty the caller's copy so it is not freed twice.
    new_text->data = NULL;
    new_text->length = 0;
    new_text->capacity = 0;

    snprintf(ctx->source_title, sizeof ctx->source_title, "%s", title);
    snprintf(ctx->source_details, sizeof ctx->source_details, "%s", details);
    log_msg(LOG_INFO, "context", "loaded \"%s\" (%zu chars)", ctx->source_title,
            ctx->text.length);
    return 1;
}

void analysis_free(TextAnalysis *ctx)
{
    textbuf_free(&ctx->text);
    table_free(&ctx->table);
}
