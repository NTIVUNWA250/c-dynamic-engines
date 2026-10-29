#ifndef TEXTIO_H
#define TEXTIO_H

#include "textbuf.h"

// Reads pasted text line by line into buf until an empty line or the end of input.
// Returns 1 if some text was read, 0 if it was empty, -1 if memory ran out.
int read_paragraph(TextBuffer *buf);

#endif
