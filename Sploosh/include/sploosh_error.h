#ifndef SPLOOSH_ERROR__H
#define SPLOOSH_ERROR_H

typedef enum sploosh_error {
	SPLOOSH_NOERROR = 0,
	SPLOOSH_LOGFAILURE,
	SPLOOSH_LASTERROR
} sploosh_error_t;

const char *sploosh_error_tostring(sploosh_error_t error);

#endif // SPLOOSH_ERROR_H
