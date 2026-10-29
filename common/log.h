#ifndef LOG_H
#define LOG_H

// Log levels, lowest to highest. Only WARN and ERROR show unless APP_LOG says otherwise.
typedef enum { LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR } LogLevel;

// Prints "[LEVEL] tag: message" to stderr when the level is enabled.
void log_msg(LogLevel level, const char *tag, const char *format, ...);

// Overrides the level chosen from the APP_LOG environment variable.
void log_set_level(LogLevel level);

#endif
