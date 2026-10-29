#include "log.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *LEVEL_NAMES[] = {"DEBUG", "INFO", "WARN", "ERROR"};

static LogLevel current_level = LOG_WARN;
static int level_loaded = 0;

static void load_level(void)
{
    const char *env = getenv("APP_LOG");
    level_loaded = 1;
    if (env == NULL)
        return;
    static const char *ENV_NAMES[] = {"debug", "info", "warn", "error"};
    for (int i = 0; i <= LOG_ERROR; i++)
        if (strcmp(env, ENV_NAMES[i]) == 0)
            current_level = (LogLevel)i;
}

void log_set_level(LogLevel level)
{
    current_level = level;
    level_loaded = 1;
}

void log_msg(LogLevel level, const char *tag, const char *format, ...)
{
    if (!level_loaded)
        load_level();
    if (level < current_level)
        return;

    va_list args;
    va_start(args, format);
    fprintf(stderr, "[%s] %s: ", LEVEL_NAMES[level], tag);
    vfprintf(stderr, format, args);
    fputc('\n', stderr);
    va_end(args);
}
