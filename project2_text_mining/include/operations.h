#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "context.h"

// Every menu operation has this shape, so they can be dispatched from one table.
typedef void (*AnalysisOperation)(TextAnalysis *ctx);

// Required operations (src/operations.c).
void count_words(TextAnalysis *ctx);
void longest_word(TextAnalysis *ctx);
void most_frequent(TextAnalysis *ctx);
void display_word_statistics(TextAnalysis *ctx);
void sort_word_statistics(TextAnalysis *ctx);
void search_word(TextAnalysis *ctx);
void generate_text_summary(TextAnalysis *ctx);

// Custom analysis and memory inspection (src/inspect.c).
void custom_analysis(TextAnalysis *ctx);
void memory_inspection(TextAnalysis *ctx);

// Text loading (src/load_text.c).
void enter_paragraph(TextAnalysis *ctx);
void show_paragraph(TextAnalysis *ctx);

#endif
