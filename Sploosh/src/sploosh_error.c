#include "../include/sploosh_error.h"

static const char *sploosh_errstrs[] = {
	[SPLOOSH_NO_ERROR] = "[SPLOOSH_NO_ERROR] No error",
	[SPLOOSH_LOG_OPEN_FAILED] = "[SPLOOSH_LOG_OPEN_FAILED] Couldn't open log for writing",
	[SPLOOSH_LOG_CLOSE_FAILED] = "[SPLOOSH_LOG_CLOSE_FAILED] Couldn't close log",
	[SPLOOSH_PRINTF_FAILED] = "[SPLOOSH_PRINTF_FAILED] printf() failed",
	[SPLOOSH_PLUGINS_MAXREACHED] = "[SPLOOSH_PLUGINS_MAXREACHED] Maximum number of plugins reached",
	[SPLOOSH_PLUGINS_NOTINLIST] = "[SPLOOSH_PLUGINS_NOTINLIST] Couldn't find plugin in list",
	[SPLOOSH_PLUGINS_ALREADYLOADED] = "[SPLOOSH_PLUGINS_ALREADYLOADED] Plugin is already loaded",
	[SPLOOSH_CONFIG_COULDNTLOAD] = "[SPLOOSH_CONFIG_COULDNTLOAD] Couldn't load config file"
};

const char *sploosh_error_tostring(sploosh_error_t error) {
	if(error < SPLOOSH_LAST_ERROR)
		return sploosh_errstrs[error];

	return sploosh_errstrs[SPLOOSH_NO_ERROR];
}
