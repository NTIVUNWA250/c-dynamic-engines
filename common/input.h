#ifndef INPUT_H
#define INPUT_H

#include <stddef.h>

int read_line(const char *prompt, char *buf, size_t size);

int read_text(const char *prompt, char *buf, size_t size);

int read_int(const char *prompt, int min, int max, int *out);

int read_float(const char *prompt, float min, float max, float *out);

int parse_int(const char *text, int min, int max, int *out);
int parse_float(const char *text, float min, float max, float *out);

char *trim(char *text);

#endif
