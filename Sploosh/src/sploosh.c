#include "../include/sploosh.h"
#include "../include/sploosh_error.h"
#include "../include/sploosh_log.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void sploosh_printusage(void) {
	puts("Usage: sploosh [options] name");
	puts("Options:");
	puts("  -v, --version     :Display version information.");
	puts("      --help        :Show this information.");
	puts("  -e, --error ERROR :Get a generic error message from error number ERROR.");
	puts("");
	puts("Report issues to <https://github.com/CJKay/Sploosh/issues>");
}

void sploosh_printversion(void) {
	printf("Sploosh %i.%i.%i\n", libmod_application.stub.version.major, libmod_application.stub.version.minor, libmod_application.stub.version.revision);
	puts("Copyright 2012 Chris Kay/nFemto Software");
	puts("License decision pending.");
	puts("This is free software: you are free to change and redistribute it.");
	puts("There is NO WARRANTY, to the extent permitted by law.");
}

int main(int argc, char *argv[]) {
	if(argc < 2 || strcmp(argv[1], "--help") == 0) {
		sploosh_printusage();

		return SPLOOSH_NO_ERROR;
	} else if(strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
		sploosh_printversion();

		return SPLOOSH_NO_ERROR;
	} else if(strcmp(argv[1], "-e") == 0 || strcmp(argv[1], "--error") == 0) {
		if(argc < 3 || !isdigit(argv[2][0])) {
			sploosh_printusage();

			return SPLOOSH_NO_ERROR;
		} else {
			sploosh_error_t error = atoi(argv[2]);
			if(error < SPLOOSH_LAST_ERROR) {
				printf("%s.\n", sploosh_error_tostring(error));
			} else {
				puts("Invalid error number.");
			}

			return SPLOOSH_NO_ERROR;
		}
	} else if(argc <= 2 && argv[1][0] == '-') {
		if(argc >= 2)
			printf("Unknown option: %s\n", argv[1]);

		sploosh_printusage();

		return SPLOOSH_NO_ERROR;
	}

	char logfile[strlen(argv[1]) + 9];
	strcpy(logfile, argv[1]);
	strcat(logfile, "/log.txt");

	sploosh_error_t error;

	sploosh_log_t log;
	if((error = sploosh_log_open(&log, logfile)) != SPLOOSH_NO_ERROR)
		return SPLOOSH_LOG_OPEN_FAILED;

	if((error = sploosh_log_close(&log)) != SPLOOSH_NO_ERROR)
		return SPLOOSH_LOG_CLOSE_FAILED;

	return SPLOOSH_NO_ERROR;
}
