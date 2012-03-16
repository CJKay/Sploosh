#ifndef SPLOOSH_LOG_H
#define SPLOOSH_LOG_H

#include "../include/sploosh_error.h"

#include <stdio.h>

typedef struct sploosh_log {
	FILE *file;
} sploosh_log_t;

sploosh_error_t sploosh_log_open(sploosh_log_t *log, const char *file);
sploosh_error_t sploosh_log_printf(sploosh_log_t *log, const char *tag, const char *format, ...);
sploosh_error_t sploosh_log_close(sploosh_log_t *log);

#endif // SPLOOSH_LOG_H
