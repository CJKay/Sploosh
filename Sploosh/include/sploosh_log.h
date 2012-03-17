#ifndef SPLOOSH_LOG_H
#define SPLOOSH_LOG_H

#include "../include/sploosh_error.h"

#include <stdio.h>

#define SPLOOSH_LOG_BUFFERSIZE 512

typedef struct sploosh_log {
	FILE *file;
} sploosh_log_t;

typedef enum sploosh_log_statement {
	SPLOOSH_LOG_NOTICE = 0,
	SPLOOSH_LOG_WARNING,
	SPLOOSH_LOG_ERROR,
	SPLOOSH_LOG_CUSTOM
} sploosh_log_statement_t;

sploosh_error_t sploosh_log_open(sploosh_log_t *log, const char *file);
sploosh_error_t sploosh_log_printf(sploosh_log_t *log, sploosh_log_statement_t tag, const char *format, ...);
sploosh_error_t sploosh_log_puts(sploosh_log_t *log, sploosh_log_statement_t tag, const char *str);
sploosh_error_t sploosh_log_eprintf(sploosh_log_t *log, sploosh_log_statement_t tag, unsigned int line, const char *file, const char *format, ...);
sploosh_error_t sploosh_log_eputs(sploosh_log_t *log, sploosh_log_statement_t tag, unsigned int line, const char *file, const char *str);
sploosh_error_t sploosh_log_close(sploosh_log_t *log);

#endif // SPLOOSH_LOG_H
