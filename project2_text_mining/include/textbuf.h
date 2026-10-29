#ifndef TEXTBUF_H
#define TEXTBUF_H

#include <stddef.h>

typedef struct TextBuffer {
    char *data;
    size_t length;
    size_t capacity;
} TextBuffer;

int textbuf_init(TextBuffer *buf, size_t initial_capacity);

int textbuf_append(TextBuffer *buf, const char *text, size_t n);

void textbuf_free(TextBuffer *buf);

#endif
