#ifndef LOG_H
#define LOG_H

typedef enum { LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR } LogLevel;

void log_msg(LogLevel level, const char *tag, const char *format, ...);

void log_set_level(LogLevel level);

#endif
