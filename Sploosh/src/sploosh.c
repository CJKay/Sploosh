#include "../include/sploosh.h"
#include "../include/sploosh_error.h"
#include "../include/sploosh_log.h"
#include "../include/sploosh_bot.h"
#include "../include/sploosh_config.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h>

libmod_application_t libmod_application = {
	{
		"Sploosh",
		NULL,
		NULL, /* Context stub */
		{ 0, 0, 1, 0 },
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

void sploosh_signal(int signal) {
	if(((sploosh_bot_t *)libmod_application.stub.context)->log.file != NULL)
		sploosh_log_printf(&((sploosh_bot_t *)libmod_application.stub.context)->log, SPLOOSH_LOG_WARNING, "Caught interrupt signal %i.", signal);

	sploosh_config_destroy(libmod_application.stub.context);
	sploosh_log_close(&((sploosh_bot_t *)libmod_application.stub.context)->log);

	exit(SPLOOSH_NO_ERROR);
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

	signal(SIGINT, &sploosh_signal);

	char logfile[strlen(argv[1]) + 9];
	strcpy(logfile, argv[1]);
	strcat(logfile, "/log.txt");

	sploosh_bot_t bot;
	sploosh_error_t error;

	if((error = sploosh_log_open(&bot.log, logfile)) != SPLOOSH_NO_ERROR)
		return error;

	sploosh_log_printf(&bot.log, SPLOOSH_LOG_NOTICE, "Log is saved at %s.", logfile);

	libmod_application.stub.context = &bot;

	char cfgfile[strlen(argv[1]) + 12];
	strcpy(cfgfile, argv[1]);
	strcat(cfgfile, "/config.cfg");

	if((error = sploosh_config_import(&bot, cfgfile)) != SPLOOSH_NO_ERROR)
		return error;

	sploosh_config_destroy(&bot);

	sploosh_log_puts(&bot.log, SPLOOSH_LOG_NOTICE, "Shutting down.");

	if((error = sploosh_log_close(&bot.log)) != SPLOOSH_NO_ERROR)
		return error;

	return SPLOOSH_NO_ERROR;
}
