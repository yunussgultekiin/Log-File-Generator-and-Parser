#ifndef PARSER_H
#define PARSER_H

#include "common.h"

int parser_init(const char *file_name);
int parser_filter(LogLevel level);
int parser_close(void);

#endif
