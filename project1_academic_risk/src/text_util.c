#include "text_util.h"
#include "student.h"

#include <ctype.h>
#include <string.h>

void to_lower_copy(char *dest, const char *src, size_t size)
{
    if (size == 0)
        return;
    char *out = dest;
    char *last = dest + size - 1;
    while (*src != '\0' && out < last)
        *out++ = (char)tolower((unsigned char)*src++);
    *out = '\0';
}

int compare_ignore_case(const char *a, const char *b)
{
    char lower_a[NAME_LEN];
    char lower_b[NAME_LEN];
    to_lower_copy(lower_a, a, sizeof lower_a);
    to_lower_copy(lower_b, b, sizeof lower_b);
    return strcmp(lower_a, lower_b);
}
