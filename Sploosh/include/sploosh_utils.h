#ifndef SPLOOSH_UTILS_H
#define SPLOOSH_UTILS_H

#include "snprintf.h"

#include <string.h>

#define sploosh_utils_snprintf snprintf
#define sploosh_utils_vsnprintf vsnprintf
#define sploosh_utils_asprintf asprintf
#define sploosh_utils_vasprintf vasprintf
#define sploosh_utils_asnprintf asnprintf
#define sploosh_utils_vasnprintf vasnprintf

int sploosh_utils_strbeginsw(const char *str, const char *substr) {
	return strncmp(str, substr, strlen(substr));
}

int sploosh_utils_strendsw(const char *str, const char *substr) {
	return strncmp(str + strlen(str) - 1 - strlen(substr), substr, strlen(substr));
}

#endif
