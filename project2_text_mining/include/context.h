#ifndef CONTEXT_H
#define CONTEXT_H

#include "textbuf.h"
#include "wordstat.h"

#define SOURCE_LEN 160
#define MIN_RECOMMENDED_WORDS 150

typedef struct TextAnalysis {
    TextBuffer text;
    WordTable table;
    char source_title[SOURCE_LEN];
    char source_details[SOURCE_LEN];
} TextAnalysis;

int analysis_init(TextAnalysis *ctx);

int analysis_replace_text(TextAnalysis *ctx, TextBuffer *new_text, const char *title,
                          const char *details);

void analysis_free(TextAnalysis *ctx);

#endif
