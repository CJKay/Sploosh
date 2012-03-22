#include "../include/sploosh_log.h"

#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

static const char *sploosh_log_stmtstrs[] = {
	[SPLOOSH_LOG_NOTICE] = "NOTICE",
	[SPLOOSH_LOG_WARNING] = "WARNING",
	[SPLOOSH_LOG_ERROR] = "ERROR"
};

sploosh_error_t sploosh_log_open(sploosh_log_t *log, const char *file) {
	if((log->file = fopen(file, "wt")) == NULL) {
		perror("Unable to open log");
		return SPLOOSH_LOG_OPEN_FAILED;
	}

	return SPLOOSH_NO_ERROR;
}

sploosh_error_t sploosh_log_printf(sploosh_log_t *log, sploosh_log_statement_t stmt, const char *format, ...) {
	char intermediate[SPLOOSH_LOG_BUFFERSIZE];

	va_list argp;

	va_start(argp, format);
	vsprintf(intermediate, format, argp);
	va_end(argp);

	sploosh_log_puts(log, stmt, intermediate);

	return SPLOOSH_NO_ERROR;
}

sploosh_error_t sploosh_log_puts(sploosh_log_t *log, sploosh_log_statement_t stmt, const char *str) {
	if(log->file == NULL) {
#ifndef NDEBUG
		fprintf(stderr, "Log has already been closed. Don't write to it!\n");
#endif
		return SPLOOSH_LOG_OPEN_FAILED;
	}

	time_t rawtime;
	struct tm *ptime;

	time(&rawtime);
	ptime = gmtime(&rawtime);

	if(	printf("[%02i:%02i:%02i %s] %s\n", ptime->tm_hour, ptime->tm_min, ptime->tm_sec, sploosh_log_strstmt(log, stmt), str) < 0 ||
		fprintf(log->file, "[%02i:%02i:%02i %s] %s\n", ptime->tm_hour, ptime->tm_min, ptime->tm_sec, sploosh_log_strstmt(log, stmt), str
	) < 0)
		return SPLOOSH_PRINTF_FAILED;

	fflush(log->file);

	return SPLOOSH_NO_ERROR;
}

sploosh_error_t sploosh_log_eprintf(sploosh_log_t *log, sploosh_log_statement_t stmt, unsigned int line, const char *file, const char *format, ...) {
	char intermediate[SPLOOSH_LOG_BUFFERSIZE];

	va_list argp;

	va_start(argp, format);
	vsprintf(intermediate, format, argp);
	va_end(argp);

	sploosh_log_eputs(log, stmt, line, file , intermediate);

	return SPLOOSH_NO_ERROR;
}

sploosh_error_t sploosh_log_eputs(sploosh_log_t *log, sploosh_log_statement_t stmt, unsigned int line, const char *file, const char *str) {
	if(log->file == NULL) {
#ifndef NDEBUG
		fprintf(stderr, "Log has already been closed. Don't write to it!\n");
#endif
		return SPLOOSH_LOG_OPEN_FAILED;
	}

	time_t rawtime;
	struct tm *ptime;

	time(&rawtime);
	ptime = gmtime(&rawtime);

	if(	fprintf(stderr, "[%02i:%02i:%02i %s][%s:%i] %s\n", ptime->tm_hour, ptime->tm_min, ptime->tm_sec, sploosh_log_strstmt(log, stmt), file, line, str) < 0 ||
		fprintf(log->file, "[%02i:%02i:%02i %s][%s:%i] %s\n", ptime->tm_hour, ptime->tm_min, ptime->tm_sec, sploosh_log_strstmt(log, stmt), file, line, str) < 0
	)
		return SPLOOSH_PRINTF_FAILED;

	fflush(log->file);

	return SPLOOSH_NO_ERROR;
}

sploosh_error_t sploosh_log_close(sploosh_log_t *log) {
	if(log->file != NULL && fclose(log->file))
		return SPLOOSH_LOG_CLOSE_FAILED;

	log->file = NULL;

	return SPLOOSH_NO_ERROR;
}

const char *sploosh_log_strstmt(sploosh_log_t *log, sploosh_log_statement_t stmt) {
	if(stmt >= SPLOOSH_LOG_CUSTOM)
		return log->statement;

	return sploosh_log_stmtstrs[stmt];
}

void sploosh_log_setstmt(sploosh_log_t *log, const char *stmt) {
	log->statement = stmt;
}
