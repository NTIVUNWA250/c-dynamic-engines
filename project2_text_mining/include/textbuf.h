#ifndef TEXTBUF_H
#define TEXTBUF_H

#include <stddef.h>

// Growable, always '\0'-terminated text on the heap.
typedef struct TextBuffer {
    char *data;
    size_t length;   // characters stored, not counting the '\0'
    size_t capacity; // bytes allocated, including room for the '\0'
} TextBuffer;

// Allocates an empty buffer with malloc. Returns 1 on success.
int textbuf_init(TextBuffer *buf, size_t initial_capacity);

// Appends n characters, growing the buffer with realloc. Returns 0 if memory runs
// out, in which case the existing text is kept unchanged.
int textbuf_append(TextBuffer *buf, const char *text, size_t n);

// Frees the text and resets the buffer to empty.
void textbuf_free(TextBuffer *buf);

#endif
