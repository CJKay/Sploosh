#ifndef SPLOOSH_ERROR_H
#define SPLOOSH_ERROR_H

typedef enum sploosh_error {
	SPLOOSH_NO_ERROR = 0,
	SPLOOSH_LOG_OPEN_FAILED,
	SPLOOSH_LOG_CLOSE_FAILED,
	SPLOOSH_PRINTF_FAILED,
	SPLOOSH_PLUGINS_MAXREACHED,
	SPLOOSH_PLUGINS_NOTINLIST,
	SPLOOSH_PLUGINS_ALREADYLOADED,
	SPLOOSH_PLUGINS_NOTFOUND,
	SPLOOSH_CONFIG_COULDNTLOAD,
	SPLOOSH_CONFIG_MISSINGVALUE,
	SPLOOSH_IRC_BADSESSION,
	SPLOOSH_IRC_COULDNTCONNECT,
	SPLOOSH_IRC_GENERICERROR,
	SPLOOSH_LAST_ERROR
} sploosh_error_t;

#ifdef SPLOOSH_EXECUTABLE

const char *sploosh_error_tostring(sploosh_error_t error);

#endif

#endif /* SPLOOSH_ERROR_H */
