#include "test.h"
#include "textbuf.h"

#include <string.h>

// The text buffer grows as text is appended and always stays '\0'-terminated.
int main(void)
{
    TextBuffer buf;
    CHECK(textbuf_init(&buf, 0));
    CHECK(buf.capacity >= 64 && buf.length == 0 && buf.data[0] == '\0');

    CHECK(textbuf_append(&buf, "Hello", 5));
    CHECK(textbuf_append(&buf, ", world", 7));
    CHECK(strcmp(buf.data, "Hello, world") == 0 && buf.length == 12);

    // Append far more than the starting capacity, one line at a time.
    size_t first_capacity = buf.capacity;
    for (int i = 0; i < 1000; i++)
        CHECK(textbuf_append(&buf, "abcdefghij", 10));
    CHECK(buf.length == 12 + 10000);
    CHECK(buf.capacity > first_capacity && buf.capacity >= buf.length + 1);
    CHECK(buf.data[buf.length] == '\0');
    CHECK(strncmp(buf.data + 12, "abcdefghij", 10) == 0);

    // Appending nothing is allowed.
    CHECK(textbuf_append(&buf, "", 0));

    textbuf_free(&buf);
    CHECK(buf.data == NULL && buf.length == 0);
    return TEST_DONE();
}
