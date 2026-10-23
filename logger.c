#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "logger.h"

static FILE *log_file = NULL;

static int ensure_log_dir(void) {
    struct stat info;

    if (mkdir(LOG_DIR, 0755) == 0) {
        return 0;
    }

    if (errno == EEXIST && stat(LOG_DIR, &info) == 0 && S_ISDIR(info.st_mode)) {
        return 0;
    }

    fprintf(stderr, "Error: Cannot create directory %s: %s\n", LOG_DIR, errno == EEXIST ? "a file with that name exists" : strerror(errno));
    return -1;
}

int log_init(const char *file_prefix) {
    char date[16];
    char file_name[256];

    if (file_prefix == NULL || *file_prefix == '\0') {
        fprintf(stderr, "Error: Log file prefix is empty.\n");
        return -1;
    }

    if (ensure_log_dir() != 0 || format_now(date, sizeof(date), DATE_FORMAT) != 0) {
        return -1;
    }

    int written = snprintf(file_name, sizeof(file_name), "%s/%s_%s.log", LOG_DIR, file_prefix, date);
    if (written < 0 || (size_t)written >= sizeof(file_name)) {
        fprintf(stderr, "Error: Log file path is too long.\n");
        return -1;
    }

    log_file = fopen(file_name, "a");
    if (log_file == NULL) {
        fprintf(stderr, "Error: Cannot open %s: %s\n", file_name, strerror(errno));
        return -1;
    }
    return 0;
}

int log_write(LogLevel level, const char *message) {
    const char *level_name = log_level_name(level);
    char timestamp[32];

    if (log_file == NULL || message == NULL || level_name == NULL) {
        fprintf(stderr, "Error: Logger is not ready or the entry is invalid.\n");
        return -1;
    }

    if (format_now(timestamp, sizeof(timestamp), TIMESTAMP_FORMAT) != 0) {
        return -1;
    }

    if (fprintf(log_file, "[%s] [%s] %s\n", timestamp, level_name, message) < 0 || fflush(log_file) != 0) {
        fprintf(stderr, "Error: Cannot write log entry: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

int log_close(void) {
    int status = 0;

    if (log_file == NULL) {
        return 0;
    }

    if (fclose(log_file) != 0) {
        fprintf(stderr, "Error: Cannot close log file: %s\n", strerror(errno));
        status = -1;
    }

    log_file = NULL;
    return status;
}
