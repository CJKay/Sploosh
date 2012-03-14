#include "../include/sploosh_error.h"

static const char *sploosh_errstrs[] = {
	[SPLOOSH_NOERROR] = "[SPLOOSH_NOERROR] No error"
};

const char *sploosh_error_tostring(sploosh_error_t error) {
	if(error < SPLOOSH_LASTERROR)
		return sploosh_errstrs[error];

	return sploosh_errstrs[SPLOOSH_NOERROR];
}
