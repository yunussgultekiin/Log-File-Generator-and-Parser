#ifndef LOGGER_H
#define LOGGER_H

#include "common.h"

int log_init(const char *file_prefix);
int log_write(LogLevel level, const char *message);
int log_close(void);

#endif
