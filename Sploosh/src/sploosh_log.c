#include "../include/sploosh_log.h"
#include "../include/sploosh_bot.h"

#include "../include/snprintf.h"

#include <libmod/libmod.h>

#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

extern libmod_application_t libmod_application;

static const char *sploosh_log_stmtstrs[] = {
	[SPLOOSH_LOG_NOTICE] = "NOTICE",
	[SPLOOSH_LOG_WARNING] = "WARNING",
	[SPLOOSH_LOG_ERROR] = "ERROR"
};

sploosh_error_t sploosh_log_open(const char *file) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	if((bot->log.file = fopen(file, "wt")) == NULL) {
		perror("Unable to open log");
		return SPLOOSH_LOG_OPEN_FAILED;
	}

	return SPLOOSH_NO_ERROR;
}

sploosh_error_t sploosh_log_printf(sploosh_log_statement_t stmt, const char *format, ...) {
	char intermediate[SPLOOSH_LOG_BUFFERSIZE];

	va_list argp;

	va_start(argp, format);
	vsnprintf(intermediate, SPLOOSH_LOG_BUFFERSIZE, format, argp);
	va_end(argp);

	sploosh_log_puts(stmt, intermediate);

	return SPLOOSH_NO_ERROR;
}

sploosh_error_t sploosh_log_puts(sploosh_log_statement_t stmt, const char *str) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	if(bot->log.file == NULL) {
#ifndef NDEBUG
		fprintf(stderr, "Log has already been closed. Don't write to it!\n");
#endif
		return SPLOOSH_LOG_OPEN_FAILED;
	}

	time_t rawtime;
	struct tm *ptime;

	time(&rawtime);
	ptime = gmtime(&rawtime);

	if(	printf("[%02i:%02i:%02i %s] %s\n", ptime->tm_hour, ptime->tm_min, ptime->tm_sec, sploosh_log_strstmt(stmt), str) < 0 ||
		fprintf(bot->log.file, "[%02i:%02i:%02i %s] %s\n", ptime->tm_hour, ptime->tm_min, ptime->tm_sec, sploosh_log_strstmt(stmt), str
	) < 0)
		return SPLOOSH_PRINTF_FAILED;

	fflush(bot->log.file);

	return SPLOOSH_NO_ERROR;
}

sploosh_error_t sploosh_log_eprintf(sploosh_log_statement_t stmt, unsigned int line, const char *file, const char *format, ...) {
	char intermediate[SPLOOSH_LOG_BUFFERSIZE];

	va_list argp;

	va_start(argp, format);
	vsnprintf(intermediate, SPLOOSH_LOG_BUFFERSIZE, format, argp);
	va_end(argp);

	sploosh_log_eputs(stmt, line, file , intermediate);

	return SPLOOSH_NO_ERROR;
}

sploosh_error_t sploosh_log_eputs(sploosh_log_statement_t stmt, unsigned int line, const char *file, const char *str) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	if(bot->log.file == NULL) {
#ifndef NDEBUG
		fprintf(stderr, "Log has already been closed. Don't write to it!\n");
#endif
		return SPLOOSH_LOG_OPEN_FAILED;
	}

	time_t rawtime;
	struct tm *ptime;

	time(&rawtime);
	ptime = gmtime(&rawtime);

	if(	fprintf(stderr, "[%02i:%02i:%02i %s][%s:%i] %s\n", ptime->tm_hour, ptime->tm_min, ptime->tm_sec, sploosh_log_strstmt(stmt), file, line, str) < 0 ||
		fprintf(bot->log.file, "[%02i:%02i:%02i %s][%s:%i] %s\n", ptime->tm_hour, ptime->tm_min, ptime->tm_sec, sploosh_log_strstmt(stmt), file, line, str) < 0
	)
		return SPLOOSH_PRINTF_FAILED;

	fflush(bot->log.file);

	return SPLOOSH_NO_ERROR;
}

sploosh_error_t sploosh_log_close(void) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	if(bot->log.file != NULL && fclose(bot->log.file))
		return SPLOOSH_LOG_CLOSE_FAILED;

	bot->log.file = NULL;

	return SPLOOSH_NO_ERROR;
}

const char *sploosh_log_strstmt(sploosh_log_statement_t stmt) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	if(stmt >= SPLOOSH_LOG_CUSTOM)
		return bot->log.statement;

	return sploosh_log_stmtstrs[stmt];
}

void sploosh_log_setstmt(const char *stmt) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	bot->log.statement = stmt;
}
