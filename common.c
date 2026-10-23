#include <stdio.h>
#include <string.h>
#include <time.h>
#include "common.h"

static const char *const level_names[LOG_LEVEL_COUNT] = {
    [LOG_INFO] = "INFO",
    [LOG_WARNING] = "WARNING",
    [LOG_ERROR] = "ERROR"
};

const char *log_level_name(LogLevel level) {
    if ((unsigned)level >= LOG_LEVEL_COUNT) {
        return NULL;
    }
    return level_names[level];
}

int log_level_from_name(const char *name, LogLevel *level) {
    if (name == NULL || level == NULL) {
        return -1;
    }

    for (unsigned index = 0; index < LOG_LEVEL_COUNT; index++) {
        if (strcmp(name, level_names[index]) == 0) {
            *level = (LogLevel)index;
            return 0;
        }
    }
    return -1;
}

int format_now(char *buffer, size_t buffer_size, const char *format) {
    time_t now = time(NULL);
    struct tm local;

    if (buffer == NULL || buffer_size == 0) {
        return -1;
    }

    buffer[0] = '\0';
    if (now == (time_t)-1 || localtime_r(&now, &local) == NULL) {
        fprintf(stderr, "Error: Cannot read local time.\n");
        return -1;
    }

    if (strftime(buffer, buffer_size, format, &local) == 0) {
        buffer[0] = '\0';
        fprintf(stderr, "Error: Cannot format time with \"%s\".\n", format);
        return -1;
    }
    return 0;
}
