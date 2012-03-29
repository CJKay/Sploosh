#ifndef SPLOOSH_UTILS_H
#define SPLOOSH_UTILS_H

#include "snprintf.h"

typedef struct sploosh_utils {
	int (*snprintf)(char *str, size_t str_m, const char *fmt, ...);
	int (*vsnprintf)(char *str, size_t str_m, const char *fmt, va_list ap);
	int (*asprintf)(char **ptr, const char *fmt, ...);
	int (*vasprintf)(char **ptr, const char *fmt, va_list ap);
	int (*asnprintf)(char **ptr, size_t str_m, const char *fmt, ...);
	int (*vasnprintf)(char **ptr, size_t str_m, const char *fmt, va_list ap);
	int (*strbeginsw)(const char *str, const char *substr);
	int (*strendsw)(const char *str, const char *substr);
} sploosh_utils_t;

#define sploosh_utils_snprintf snprintf
#define sploosh_utils_vsnprintf vsnprintf
#define sploosh_utils_asprintf asprintf
#define sploosh_utils_vasprintf vasprintf
#define sploosh_utils_vasnprintf vasnprintf

int sploosh_utils_strbeginsw(const char *str, const char *substr) {
	return strncmp(str, substr, strlen(substr));
}

int sploosh_utils_strendsw(const char *str, const char *substr) {
	return strncmp(str + strlen(str) - 1 - strlen(substr), substr, strlen(substr));
}

#endif
