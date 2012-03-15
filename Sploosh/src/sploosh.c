#include "../include/sploosh_error.h"
#include "../include/sploosh.h"

#include <libmod/libmod.h>

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

libmod_application_t libmod_application = {
		{
				"Sploosh",
				NULL,
				NULL, // Context stub
				{0, 0, 1, 0},
				{ 0,
						{ }
				}
		}
};

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

		return SPLOOSH_NOERROR;
	} else if(strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
		sploosh_printversion();

		return SPLOOSH_NOERROR;
	} else if(strcmp(argv[1], "-e") == 0 || strcmp(argv[1], "--error") == 0) {
		if(argc < 3 || !isdigit(argv[2][0])) {
			sploosh_printusage();

			return SPLOOSH_NOERROR;
		} else {
			sploosh_error_t error = atoi(argv[2]);
			if(error < SPLOOSH_LASTERROR) {
				printf("%s.\n", sploosh_error_tostring(error));
			} else {
				puts("Invalid error number.");
			}

			return SPLOOSH_NOERROR;
		}
	} else {
		if(argc >= 2)
			printf("Unknown option: %s\n", argv[1]);

		sploosh_printusage();

		return SPLOOSH_NOERROR;
	}

	return SPLOOSH_NOERROR;
}
