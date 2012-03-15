#ifndef SPLOOSH_LOG_H
#define SPLOOSH_LOG_H

typedef struct sploosh_log {
	FILE *file;
	FILE *out;
	FILE *error;
} sploosh_log_t;

#endif // SPLOOSH_LOG_H
