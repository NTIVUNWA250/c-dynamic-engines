#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "context.h"

typedef void (*AnalysisOperation)(TextAnalysis *ctx);

void count_words(TextAnalysis *ctx);
void longest_word(TextAnalysis *ctx);
void most_frequent(TextAnalysis *ctx);
void display_word_statistics(TextAnalysis *ctx);
void sort_word_statistics(TextAnalysis *ctx);
void search_word(TextAnalysis *ctx);
void generate_text_summary(TextAnalysis *ctx);

void custom_analysis(TextAnalysis *ctx);
void memory_inspection(TextAnalysis *ctx);

void enter_paragraph(TextAnalysis *ctx);
void show_paragraph(TextAnalysis *ctx);

#endif
