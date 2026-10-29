#ifndef TEXT_UTIL_H
#define TEXT_UTIL_H

#include <stddef.h>

// Copies src into dest in lowercase, never writing more than size bytes.
void to_lower_copy(char *dest, const char *src, size_t size);

// Case-insensitive strcmp, built on lowercase copies and strcmp.
int compare_ignore_case(const char *a, const char *b);

#endif
