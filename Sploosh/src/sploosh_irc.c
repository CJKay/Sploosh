#include "../include/sploosh_irc.h"
#include "../include/sploosh_log.h"
#include "../include/sploosh_bot.h"
#include "../include/sploosh_config.h"

#include <libmod/libmod.h>

#include <assert.h>
#include <signal.h>

extern libmod_application_t libmod_application;

void sploosh_irc_signal(int signal) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	if(((sploosh_bot_t *)libmod_application.stub.context)->log.file != NULL)
		sploosh_log_printf(SPLOOSH_LOG_WARNING, "Caught interrupt signal %i. Disconnecting...", signal);

	if(irc_is_connected(bot->irc.session))
		irc_destroy_session(bot->irc.session);

	sploosh_config_destroy();
	sploosh_log_close();

	exit(SPLOOSH_NO_ERROR);
}

void sploosh_irc_connected(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) {
	sploosh_log_puts(SPLOOSH_LOG_NOTICE, "Connection successful!");
}

sploosh_error_t sploosh_irc_run(void) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	assert(bot->irc.session == NULL);

	irc_callbacks_t callbacks = { };
	callbacks.event_connect = &sploosh_irc_connected;

	if((bot->irc.session = irc_create_session(&callbacks)) == 0) {
		sploosh_log_eputs(SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Couldn't create IRC session.");
		return SPLOOSH_IRC_BADSESSION;
	}

	signal(SIGINT, &sploosh_irc_signal);

	if(irc_connect(
		bot->irc.session,
		bot->info.server,
		bot->info.port,
		bot->info.password,
		bot->info.nickname,
		bot->info.username,
		bot->info.realname
	) != 0) {
		sploosh_log_eprintf(SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Couldn't connect to %s: %s", bot->info.server, irc_strerror(irc_errno(bot->irc.session)));

		irc_destroy_session(bot->irc.session);
		bot->irc.session = NULL;

		return SPLOOSH_IRC_COULDNTCONNECT;
	}

	sploosh_log_puts(SPLOOSH_LOG_NOTICE, "Connecting...");

	if(irc_run(bot->irc.session) != 0) {
		sploosh_log_eprintf(SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "IRC error: %s", irc_strerror(irc_errno(bot->irc.session)));

		irc_destroy_session(bot->irc.session);
		bot->irc.session = NULL;

		return SPLOOSH_IRC_GENERICERROR;
	}

	sploosh_log_puts(SPLOOSH_LOG_NOTICE, "Disconnected from the server.");

	irc_destroy_session(bot->irc.session);
	bot->irc.session = NULL;

	return SPLOOSH_NO_ERROR;
}
