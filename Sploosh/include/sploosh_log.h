#ifndef SPLOOSH_LOG_H
#define SPLOOSH_LOG_H

#include "../include/sploosh_error.h"

#include <stdio.h>

#define SPLOOSH_LOG_BUFFERSIZE 512

typedef struct sploosh_log {
	FILE *file;
} sploosh_log_t;

sploosh_error_t sploosh_log_open(sploosh_log_t *log, const char *file);
sploosh_error_t sploosh_log_printf(sploosh_log_t *log, const char *tag, const char *format, ...);
sploosh_error_t sploosh_log_puts(sploosh_log_t *log, const char *tag, const char *str);
sploosh_error_t sploosh_log_eprintf(sploosh_log_t *log, const char *tag, unsigned int line, const char *file, const char *format, ...);
sploosh_error_t sploosh_log_eputs(sploosh_log_t *log, const char *tag, unsigned int line, const char *file, const char *str);
sploosh_error_t sploosh_log_close(sploosh_log_t *log);

#endif // SPLOOSH_LOG_H
