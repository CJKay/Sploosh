#ifndef SPLOOSH_LOG_H
#define SPLOOSH_LOG_H

#include "../include/sploosh_error.h"

typedef enum sploosh_log_statement {
	SPLOOSH_LOG_NOTICE = 0,
	SPLOOSH_LOG_WARNING,
	SPLOOSH_LOG_ERROR,
	SPLOOSH_LOG_CUSTOM
} sploosh_log_statement_t;

#ifdef SPLOOSH_EXECUTABLE

#include <stdio.h>

#define SPLOOSH_LOG_BUFFERSIZE 512

typedef struct sploosh_log {
	FILE *file;
	const char *statement;
} sploosh_log_t;

sploosh_error_t sploosh_log_open(const char *file);
sploosh_error_t sploosh_log_printf(sploosh_log_statement_t stmt, const char *format, ...);
sploosh_error_t sploosh_log_puts(sploosh_log_statement_t stmt, const char *str);
sploosh_error_t sploosh_log_eprintf(sploosh_log_statement_t stmt, unsigned int line, const char *file, const char *format, ...);
sploosh_error_t sploosh_log_eputs(sploosh_log_statement_t stmt, unsigned int line, const char *file, const char *str);
sploosh_error_t sploosh_log_close(void);
const char     *sploosh_log_strstmt(sploosh_log_statement_t stmt);
void            sploosh_log_setstmt(const char *stmt);

#endif

#endif /* SPLOOSH_LOG_H */
