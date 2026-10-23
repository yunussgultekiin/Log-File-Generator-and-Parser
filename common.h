#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>

#define LOG_DIR "logs"
#define LOG_FILE_PREFIX "app"
#define DATE_FORMAT "%Y%m%d"
#define TIMESTAMP_FORMAT "%Y-%m-%d %H:%M:%S"

typedef enum LogLevel {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
    LOG_LEVEL_COUNT
} LogLevel;

const char *log_level_name(LogLevel level);
int log_level_from_name(const char *name, LogLevel *level);
int format_now(char *buffer, size_t buffer_size, const char *format);

#endif
