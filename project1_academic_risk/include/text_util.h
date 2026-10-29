#ifndef TEXT_UTIL_H
#define TEXT_UTIL_H

#include <stddef.h>

void to_lower_copy(char *dest, const char *src, size_t size);

int compare_ignore_case(const char *a, const char *b);

#endif
