#ifndef INPUT_H
#define INPUT_H

#include <stddef.h>

// All readers return 1 on success and 0 when input has ended (EOF).

// Reads one line into buf without overflowing it; extra characters are dropped.
int read_line(const char *prompt, char *buf, size_t size);

// Reads a non-empty line, trimmed of surrounding spaces.
int read_text(const char *prompt, char *buf, size_t size);

// Keeps asking until a whole number in [min, max] is entered.
int read_int(const char *prompt, int min, int max, int *out);

// Keeps asking until a number in [min, max] is entered.
int read_float(const char *prompt, float min, float max, float *out);

// Converts text to a number in range. Return 1 if valid, 0 otherwise.
int parse_int(const char *text, int min, int max, int *out);
int parse_float(const char *text, float min, float max, float *out);

// Removes leading and trailing whitespace in place and returns text.
char *trim(char *text);

#endif
