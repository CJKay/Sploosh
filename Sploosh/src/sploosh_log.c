#include "../include/sploosh_log.h"

sploosh_error_t sploosh_log_open(sploosh_log_t *log, const char *file) {
	if((log->file = fopen(file, "wt")) == NULL) {
		perror("Unable to open log");
		return SPLOOSH_LOG_OPEN_FAILED;
	}

	return SPLOOSH_NO_ERROR;
}

sploosh_error_t sploosh_log_write(sploosh_log_t *log, const char *format, ...) {
	return SPLOOSH_NO_ERROR;
}

sploosh_error_t sploosh_log_close(sploosh_log_t *log) {
	if(fclose(log->file))
		return SPLOOSH_LOG_CLOSE_FAILED;

	return SPLOOSH_NO_ERROR;
}
