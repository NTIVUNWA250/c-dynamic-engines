#ifndef CONTEXT_H
#define CONTEXT_H

#include "textbuf.h"
#include "wordstat.h"

#define SOURCE_LEN 160
#define MIN_RECOMMENDED_WORDS 150

// Everything the menu operations work on: the text, its word table and its source.
typedef struct TextAnalysis {
    TextBuffer text;
    WordTable table;
    char source_title[SOURCE_LEN];
    char source_details[SOURCE_LEN];
} TextAnalysis;

// Allocates the text buffer and word table. Returns 1 on success.
int analysis_init(TextAnalysis *ctx);

// Takes ownership of new_text (a filled buffer), analyses it, and frees the old text.
// Returns 1 on success; on failure the previous text and results are kept.
int analysis_replace_text(TextAnalysis *ctx, TextBuffer *new_text, const char *title,
                          const char *details);

// Frees everything the context owns.
void analysis_free(TextAnalysis *ctx);

#endif
