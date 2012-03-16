#include "../include/sploosh_log.h"

#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

sploosh_error_t sploosh_log_open(sploosh_log_t *log, const char *file) {
	if((log->file = fopen(file, "wt")) == NULL) {
		perror("Unable to open log");
		return SPLOOSH_LOG_OPEN_FAILED;
	}

	return SPLOOSH_NO_ERROR;
}

sploosh_error_t sploosh_log_printf(sploosh_log_t *log, const char *tag, const char *format, ...) {
	char intermediate[strlen(format) + 128];

	va_list argp;

	va_start(argp, format);
	vsnprintf(intermediate, 256, format, argp);
	va_end(argp);

	sploosh_log_puts(log, tag, intermediate);

	return SPLOOSH_NO_ERROR;
}

sploosh_error_t sploosh_log_puts(sploosh_log_t *log, const char *tag, const char *str) {
	time_t rawtime;
	struct tm *ptime;

	time(&rawtime);
	ptime = gmtime(&rawtime);

	if(	printf("[%02i:%02i:%02i %s] %s\n", ptime->tm_hour, ptime->tm_min, ptime->tm_sec, tag, str) < 0 ||
		fprintf(log->file, "[%02i:%02i:%02i %s] %s\n", ptime->tm_hour, ptime->tm_min, ptime->tm_sec, tag, str
	) < 0)
		return SPLOOSH_PRINTF_FAILED;

	return SPLOOSH_NO_ERROR;
}

sploosh_error_t sploosh_log_eprintf(sploosh_log_t *log, const char *tag, const char *format, ...) {
	char intermediate[strlen(format) + 128];

	va_list argp;

	va_start(argp, format);
	vsnprintf(intermediate, 256, format, argp);
	va_end(argp);

	sploosh_log_eputs(log, tag, intermediate);

	return SPLOOSH_NO_ERROR;
}

sploosh_error_t sploosh_log_eputs(sploosh_log_t *log, const char *tag, const char *str) {
	time_t rawtime;
	struct tm *ptime;

	time(&rawtime);
	ptime = gmtime(&rawtime);

	if(	fprintf(stderr, "[%02i:%02i:%02i %s] %s\n", ptime->tm_hour, ptime->tm_min, ptime->tm_sec, tag, str) < 0 ||
		fprintf(log->file, "[%02i:%02i:%02i %s] %s\n", ptime->tm_hour, ptime->tm_min, ptime->tm_sec, tag, str
	) < 0)
		return SPLOOSH_PRINTF_FAILED;

	return SPLOOSH_NO_ERROR;
}

sploosh_error_t sploosh_log_close(sploosh_log_t *log) {
	if(fclose(log->file))
		return SPLOOSH_LOG_CLOSE_FAILED;

	return SPLOOSH_NO_ERROR;
}
