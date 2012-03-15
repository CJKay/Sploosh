#include "../include/sploosh_log.h"

sploosh_error_t sploosh_log_open(sploosh_log_t *log, const char *file) {
	log->file = fopen(file, "wt");
	return SPLOOSH_NOERROR;
}

sploosh_error_t sploosh_log_write(sploosh_log_t *log, const char *format, ...) {
	return SPLOOSH_NOERROR;
}

sploosh_error_t sploosh_log_close(sploosh_log_t *log) {
	fclose(log->file);
	return SPLOOSH_NOERROR;
}
