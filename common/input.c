#define _POSIX_C_SOURCE 200809L

#include "input.h"
#include "log.h"

#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void echo_if_piped(const char *line)
{
    if (!isatty(STDIN_FILENO))
        printf("%s\n", line);
}

int read_line(const char *prompt, char *buf, size_t size)
{
    if (prompt != NULL) {
        printf("%s", prompt);
        fflush(stdout);
    }
    if (size < 2 || fgets(buf, (int)size, stdin) == NULL)
        return 0;

    int truncated = 0;
    char *newline = strchr(buf, '\n');
    if (newline != NULL) {
        *newline = '\0';
    } else {
        int c = getchar();
        if (c != '\n' && c != EOF) {
            truncated = 1;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
        }
    }
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\r')
        buf[len - 1] = '\0';

    echo_if_piped(buf);
    if (truncated) {
        printf("(input too long, kept the first %zu characters)\n", size - 1);
        log_msg(LOG_WARN, "input", "line truncated to %zu characters", size - 1);
    }
    return 1;
}

int read_text(const char *prompt, char *buf, size_t size)
{
    for (;;) {
        if (!read_line(prompt, buf, size))
            return 0;
        if (trim(buf)[0] != '\0')
            return 1;
        printf("Invalid input. This field cannot be empty.\n");
    }
}

int read_int(const char *prompt, int min, int max, int *out)
{
    char buf[64];
    for (;;) {
        if (!read_line(prompt, buf, sizeof buf))
            return 0;
        if (parse_int(buf, min, max, out))
            return 1;
        printf("Invalid input. Enter a whole number from %d to %d.\n", min, max);
        log_msg(LOG_INFO, "input", "rejected integer '%s'", buf);
    }
}

int read_float(const char *prompt, float min, float max, float *out)
{
    char buf[64];
    for (;;) {
        if (!read_line(prompt, buf, sizeof buf))
            return 0;
        if (parse_float(buf, min, max, out))
            return 1;
        printf("Invalid input. Enter a number from %.1f to %.1f.\n", min, max);
        log_msg(LOG_INFO, "input", "rejected number '%s'", buf);
    }
}

int parse_int(const char *text, int min, int max, int *out)
{
    char *end;
    errno = 0;
    long value = strtol(text, &end, 10);
    if (end == text)
        return 0;
    while (isspace((unsigned char)*end))
        end++;
    if (*end != '\0' || errno == ERANGE || value < min || value > max)
        return 0;
    *out = (int)value;
    return 1;
}

int parse_float(const char *text, float min, float max, float *out)
{
    char *end;
    errno = 0;
    float value = strtof(text, &end);
    if (end == text)
        return 0;
    while (isspace((unsigned char)*end))
        end++;
    if (*end != '\0' || errno == ERANGE || !isfinite(value) || value < min || value > max)
        return 0;
    *out = value;
    return 1;
}

char *trim(char *text)
{
    char *start = text;
    while (isspace((unsigned char)*start))
        start++;
    char *end = start + strlen(start);
    while (end > start && isspace((unsigned char)end[-1]))
        end--;
    *end = '\0';
    if (start != text)
        memmove(text, start, (size_t)(end - start) + 1);
    return text;
}
