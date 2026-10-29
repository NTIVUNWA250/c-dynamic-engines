#include "textio.h"
#include "log.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

// 1 if the text holds only spaces, tabs and line breaks.
static int is_blank(const char *text)
{
    for (const char *p = text; *p != '\0'; p++)
        if (!isspace((unsigned char)*p))
            return 0;
    return 1;
}

int read_paragraph(TextBuffer *buf)
{
    char chunk[256];
    int at_line_start = 1;
    size_t lines = 0;

    // fgets reads at most 255 characters at a time, so any line length is fine:
    // long lines simply arrive in several chunks and are appended one after another.
    while (fgets(chunk, sizeof chunk, stdin) != NULL) {
        size_t n = strlen(chunk);
        int ends_line = n > 0 && chunk[n - 1] == '\n';
        if (at_line_start && ends_line && is_blank(chunk))
            break; // an empty line ends the paragraph
        if (!textbuf_append(buf, chunk, n))
            return -1;
        if (ends_line)
            lines++;
        at_line_start = ends_line;
    }
    log_msg(LOG_INFO, "textio", "read %zu chars over %zu lines", buf->length, lines);
    printf("(read %zu characters)\n", buf->length);
    return is_blank(buf->data) ? 0 : 1;
}
