#ifndef SPLOOSH_LOG_H
#define SPLOOSH_LOG_H

typedef struct sploosh_log {
	FILE *file;
	FILE *out;
	FILE *error;
} sploosh_log_t;

sploosh_error_t sploosh_log_open(sploosh_log_t *log);
sploosh_error_t sploosh_log_write(sploosh_log_t *log, const char *format, ...);
sploosh_error_t sploosh_log_close(sploosh_log_t *log);

#endif // SPLOOSH_LOG_H
