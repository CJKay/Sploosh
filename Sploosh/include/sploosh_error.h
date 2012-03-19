#ifndef SPLOOSH_ERROR_H
#define SPLOOSH_ERROR_H

typedef enum sploosh_error {
	SPLOOSH_NO_ERROR = 0,
	SPLOOSH_LOG_OPEN_FAILED,
	SPLOOSH_LOG_CLOSE_FAILED,
	SPLOOSH_PRINTF_FAILED,
	SPLOOSH_PLUGINS_MAXREACHED,
	SPLOOSH_LAST_ERROR
} sploosh_error_t;

const char *sploosh_error_tostring(sploosh_error_t error);

#endif // SPLOOSH_ERROR_H
