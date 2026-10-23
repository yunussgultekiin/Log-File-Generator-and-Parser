#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"
#include "parser.h"

static void print_usage(FILE *stream) {
    fprintf(stream, "Usage:\n");
    fprintf(stream, "  logtool --log \"message\" --level INFO|WARNING|ERROR\n");
    fprintf(stream, "  logtool --parse <file_path> --level INFO|WARNING|ERROR\n");
    fprintf(stream, "  logtool --help\n");
}

static int run_log(const char *message, LogLevel level) {
    if (log_init(LOG_FILE_PREFIX) != 0) {
        return EXIT_FAILURE;
    }

    int write_status = log_write(level, message);
    int close_status = log_close();
    return write_status == 0 && close_status == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int run_parse(const char *file_name, LogLevel level) {
    if (parser_init(file_name) != 0) {
        return EXIT_FAILURE;
    }

    int filter_status = parser_filter(level);
    int close_status = parser_close();
    return filter_status == 0 && close_status == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}

int main(int argc, char *argv[]) {
    LogLevel level;

    if (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)) {
        print_usage(stdout);
        return EXIT_SUCCESS;
    }

    if (argc != 5 || strcmp(argv[3], "--level") != 0) {
        fprintf(stderr, "Error: Invalid arguments.\n");
        print_usage(stderr);
        return EXIT_FAILURE;
    }

    if (log_level_from_name(argv[4], &level) != 0) {
        fprintf(stderr, "Error: Unknown log level: %s\n", argv[4]);
        print_usage(stderr);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "--log") == 0) {
        return run_log(argv[2], level);
    }

    if (strcmp(argv[1], "--parse") == 0) {
        return run_parse(argv[2], level);
    }

    fprintf(stderr, "Error: Unknown command: %s\n", argv[1]);
    print_usage(stderr);
    return EXIT_FAILURE;
}
