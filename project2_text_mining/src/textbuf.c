#include "textbuf.h"
#include "log.h"

#include <stdlib.h>
#include <string.h>

#define MIN_CAPACITY 64

int textbuf_init(TextBuffer *buf, size_t initial_capacity)
{
    if (initial_capacity < MIN_CAPACITY)
        initial_capacity = MIN_CAPACITY;
    buf->data = malloc(initial_capacity);
    buf->length = 0;
    buf->capacity = 0;
    if (buf->data == NULL) {
        log_msg(LOG_ERROR, "text", "could not allocate %zu bytes", initial_capacity);
        return 0;
    }
    buf->data[0] = '\0';
    buf->capacity = initial_capacity;
    return 1;
}

int textbuf_append(TextBuffer *buf, const char *text, size_t n)
{
    size_t needed = buf->length + n + 1;
    if (needed > buf->capacity) {
        // Double until it fits, so long pasted text needs few reallocations.
        size_t new_capacity = buf->capacity < MIN_CAPACITY ? MIN_CAPACITY : buf->capacity;
        while (new_capacity < needed)
            new_capacity *= 2;
        char *grown = realloc(buf->data, new_capacity);
        if (grown == NULL) {
            log_msg(LOG_ERROR, "text", "realloc to %zu bytes failed", new_capacity);
            return 0;
        }
        log_msg(LOG_INFO, "text", "buffer %zu -> %zu bytes", buf->capacity, new_capacity);
        buf->data = grown;
        buf->capacity = new_capacity;
    }
    memcpy(buf->data + buf->length, text, n);
    buf->length += n;
    buf->data[buf->length] = '\0';
    return 1;
}

void textbuf_free(TextBuffer *buf)
{
    free(buf->data);
    buf->data = NULL;
    buf->length = 0;
    buf->capacity = 0;
}
