#include "../include/sploosh_error.h"

static const char *sploosh_errstrs[] = {
	[SPLOOSH_NO_ERROR] = "[SPLOOSH_NO_ERROR] No error",
	[SPLOOSH_LOG_OPEN_FAILED] = "[SPLOOSH_LOG_OPEN_FAILED] Couldn't open log for writing",
	[SPLOOSH_LOG_CLOSE_FAILED] = "[SPLOOSH_LOG_CLOSE_FAILED] Couldn't close log"
};

const char *sploosh_error_tostring(sploosh_error_t error) {
	if(error < SPLOOSH_LASTERROR)
		return sploosh_errstrs[error];

	return sploosh_errstrs[SPLOOSH_NO_ERROR];
}
