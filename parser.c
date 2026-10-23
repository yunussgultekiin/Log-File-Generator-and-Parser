#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

static FILE *log_file = NULL;

static int line_has_level(const char *line, const char *level_name) {
    const char *timestamp_end = strstr(line, "] [");

    if (line[0] != '[' || timestamp_end == NULL) {
        return 0;
    }

    const char *tag = timestamp_end + 3;
    size_t length = strlen(level_name);
    return strncmp(tag, level_name, length) == 0 && tag[length] == ']';
}

int parser_init(const char *file_name) {
    if (file_name == NULL || *file_name == '\0') {
        fprintf(stderr, "Error: Log file path is empty.\n");
        return -1;
    }

    log_file = fopen(file_name, "r");
    if (log_file == NULL) {
        fprintf(stderr, "Error: Cannot open %s: %s\n", file_name, strerror(errno));
        return -1;
    }
    return 0;
}

int parser_filter(LogLevel level) {
    const char *level_name = log_level_name(level);
    char *line = NULL;
    size_t capacity = 0;
    int status = 0;

    if (log_file == NULL || level_name == NULL) {
        fprintf(stderr, "Error: Parser is not ready or the level is invalid.\n");
        return -1;
    }

    while (getline(&line, &capacity, log_file) != -1) {
        if (line_has_level(line, level_name) && fputs(line, stdout) == EOF) {
            status = -1;
            break;
        }
    }

    if (ferror(log_file)) {
        fprintf(stderr, "Error: Cannot read log file: %s\n", strerror(errno));
        status = -1;
    }

    free(line);
    if (fflush(stdout) != 0) {
        status = -1;
    }
    return status;
}

int parser_close(void) {
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
