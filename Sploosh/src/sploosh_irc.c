#include "../include/sploosh_irc.h"
#include "../include/sploosh_log.h"
#include "../include/sploosh_bot.h"
#include "../include/sploosh_config.h"
#include "../include/sploosh_api.h"

#include <libmod/libmod.h>

#include <assert.h>
#include <signal.h>
#include <string.h>

extern libmod_application_t libmod_application;

static void sploosh_irc_signal(int signal) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	if(((sploosh_bot_t *)libmod_application.stub.context)->log.file != NULL)
		sploosh_log_printf(SPLOOSH_LOG_WARNING, "Caught interrupt signal %d.", signal);

	if(irc_is_connected(bot->irc.session))
		irc_cmd_quit(bot->irc.session, "Disconnecting...");

	irc_destroy_session(bot->irc.session);
	sploosh_plugins_clear();
	sploosh_config_destroy();
	sploosh_log_close();

	exit(SPLOOSH_NO_ERROR);
}

static void sploosh_irc_event_connect(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	sploosh_log_puts(SPLOOSH_LOG_NOTICE, "Connection successful!");

	int i;
	for(i = 0; i < bot->plugins.count; i++) {
		sploosh_event_generic_t fn = ((sploosh_api_t *)bot->plugins.plugin[i]->appcontext)->events.connect;
		if(fn != NULL)
			fn(event, origin, params, count);
	}
}

static void sploosh_irc_event_nick(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	int i;
	for(i = 0; i < bot->plugins.count; i++) {
		sploosh_event_generic_t fn = ((sploosh_api_t *)bot->plugins.plugin[i]->appcontext)->events.nick;
		if(fn != NULL)
			fn(event, origin, params, count);
	}
}

static void sploosh_irc_event_quit(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	int i;
	for(i = 0; i < bot->plugins.count; i++) {
		sploosh_event_generic_t fn = ((sploosh_api_t *)bot->plugins.plugin[i]->appcontext)->events.quit;
		if(fn != NULL)
			fn(event, origin, params, count);
	}
}

static void sploosh_irc_event_join(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	int i;
	for(i = 0; i < bot->plugins.count; i++) {
		sploosh_event_generic_t fn = ((sploosh_api_t *)bot->plugins.plugin[i]->appcontext)->events.join;
		if(fn != NULL)
			fn(event, origin, params, count);
	}
}

static void sploosh_irc_event_part(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	int i;
	for(i = 0; i < bot->plugins.count; i++) {
		sploosh_event_generic_t fn = ((sploosh_api_t *)bot->plugins.plugin[i]->appcontext)->events.part;
		if(fn != NULL)
			fn(event, origin, params, count);
	}
}

static void sploosh_irc_event_mode(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	int i;
	for(i = 0; i < bot->plugins.count; i++) {
		sploosh_event_generic_t fn = ((sploosh_api_t *)bot->plugins.plugin[i]->appcontext)->events.mode;
		if(fn != NULL)
			fn(event, origin, params, count);
	}
}

static void sploosh_irc_event_umode(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	int i;
	for(i = 0; i < bot->plugins.count; i++) {
		sploosh_event_generic_t fn = ((sploosh_api_t *)bot->plugins.plugin[i]->appcontext)->events.umode;
		if(fn != NULL)
			fn(event, origin, params, count);
	}
}

static void sploosh_irc_event_topic(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	int i;
	for(i = 0; i < bot->plugins.count; i++) {
		sploosh_event_generic_t fn = ((sploosh_api_t *)bot->plugins.plugin[i]->appcontext)->events.topic;
		if(fn != NULL)
			fn(event, origin, params, count);
	}
}

static void sploosh_irc_event_kick(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	int i;
	for(i = 0; i < bot->plugins.count; i++) {
		sploosh_event_generic_t fn = ((sploosh_api_t *)bot->plugins.plugin[i]->appcontext)->events.kick;
		if(fn != NULL)
			fn(event, origin, params, count);
	}
}

static void sploosh_irc_event_channel(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	int i;
	for(i = 0; i < bot->plugins.count; i++) {
		sploosh_event_generic_t fn;

		if(*params[count - 1] == '!') {
			char *text = malloc(sizeof(char) * (strlen(params[count - 1] + 1) + 1));
			strcpy(text, params[count - 1] + 1);

			char **args = NULL;

			char *pch = strstr(text, " ");
			*pch = '\0';

			int i = 0;
			pch = strtok(pch + 1, " ");
			while(pch != NULL) {
				args = realloc(args, sizeof(char *) * ++i);
				args[i - 1] = pch;
				pch = strtok(NULL, " ");
			}

			fn = ((sploosh_api_t *)bot->plugins.plugin[i]->appcontext)->events.command;
			if(fn != NULL)
				fn(params[0], text, (const char **)args, i);

			free(text);
		} else {
			fn = ((sploosh_api_t *)bot->plugins.plugin[i]->appcontext)->events.channel;
			if(fn != NULL)
				fn(event, origin, params, count);
		}
	}
}

static void sploosh_irc_event_privmsg(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	int i;
	for(i = 0; i < bot->plugins.count; i++) {
		sploosh_event_generic_t fn = ((sploosh_api_t *)bot->plugins.plugin[i]->appcontext)->events.privmsg;
		if(fn != NULL)
			fn(event, origin, params, count);
	}
}

static void sploosh_irc_event_notice(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	int i;
	for(i = 0; i < bot->plugins.count; i++) {
		sploosh_event_generic_t fn = ((sploosh_api_t *)bot->plugins.plugin[i]->appcontext)->events.notice;
		if(fn != NULL)
			fn(event, origin, params, count);
	}
}

static void sploosh_irc_event_invite(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	int i;
	for(i = 0; i < bot->plugins.count; i++) {
		sploosh_event_generic_t fn = ((sploosh_api_t *)bot->plugins.plugin[i]->appcontext)->events.invite;
		if(fn != NULL)
			fn(event, origin, params, count);
	}
}

static void sploosh_irc_event_ctcp_req(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	int i;
	for(i = 0; i < bot->plugins.count; i++) {
		sploosh_event_generic_t fn = ((sploosh_api_t *)bot->plugins.plugin[i]->appcontext)->events.ctcp_req;
		if(fn != NULL)
			fn(event, origin, params, count);
	}
}

static void sploosh_irc_event_ctcp_rep(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	int i;
	for(i = 0; i < bot->plugins.count; i++) {
		sploosh_event_generic_t fn = ((sploosh_api_t *)bot->plugins.plugin[i]->appcontext)->events.ctcp_rep;
		if(fn != NULL)
			fn(event, origin, params, count);
	}
}

static void sploosh_irc_event_ctcp_action(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	int i;
	for(i = 0; i < bot->plugins.count; i++) {
		sploosh_event_generic_t fn = ((sploosh_api_t *)bot->plugins.plugin[i]->appcontext)->events.ctcp_action;
		if(fn != NULL)
			fn(event, origin, params, count);
	}
}

static void sploosh_irc_event_unknown(irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	int i;
	for(i = 0; i < bot->plugins.count; i++) {
		sploosh_event_generic_t fn = ((sploosh_api_t *)bot->plugins.plugin[i]->appcontext)->events.unknown;
		if(fn != NULL)
			fn(event, origin, params, count);
	}
}

static void sploosh_irc_event_numeric(irc_session_t *session, unsigned int event, const char *origin, const char **params, unsigned int count) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	int i;
	for(i = 0; i < bot->plugins.count; i++) {
		sploosh_event_numeric_t fn = ((sploosh_api_t *)bot->plugins.plugin[i]->appcontext)->events.numeric;
		if(fn != NULL)
			fn(event, origin, params, count);
	}
}

static void sploosh_irc_event_dcc_chat_req(irc_session_t *session, const char *nick, const char *addr, sploosh_dccid_t dccid) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	int i;
	for(i = 0; i < bot->plugins.count; i++) {
		sploosh_event_dcc_chat_req_t fn = ((sploosh_api_t *)bot->plugins.plugin[i]->appcontext)->events.dcc_chat_req;
		if(fn != NULL)
			fn(nick, addr, dccid);
	}
}

static void sploosh_irc_event_dcc_send_req(irc_session_t *session, const char *nick, const char *addr, const char *filename, unsigned long size, sploosh_dccid_t dccid) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	int i;
	for(i = 0; i < bot->plugins.count; i++) {
		sploosh_event_dcc_send_req_t fn = ((sploosh_api_t *)bot->plugins.plugin[i]->appcontext)->events.dcc_send_req;
		if(fn != NULL)
			fn(nick, addr, filename, size, dccid);
	}
}

int sploosh_irc_cmd_quit(const char *reason) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	return irc_cmd_quit(bot->irc.session, reason);
}

int sploosh_irc_cmd_join(const char *channel, const char *key) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	return irc_cmd_join(bot->irc.session, channel, key);
}

int sploosh_irc_cmd_part(const char *channel) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	return irc_cmd_part(bot->irc.session, channel);
}

int sploosh_irc_cmd_invite(const char *nick, const char *channel) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	return irc_cmd_invite(bot->irc.session, nick, channel);
}

int sploosh_irc_cmd_names(const char *channel) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	return irc_cmd_names(bot->irc.session, channel);
}

int sploosh_irc_cmd_list(const char *channel) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	return irc_cmd_list(bot->irc.session, channel);
}

int sploosh_irc_cmd_topic(const char *channel, const char *topic) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	return irc_cmd_topic(bot->irc.session, channel, topic);
}

int sploosh_irc_cmd_channel_mode(const char *channel, const char *mode) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	return irc_cmd_channel_mode(bot->irc.session, channel, mode);
}

int sploosh_irc_cmd_user_mode(const char *mode) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	return irc_cmd_user_mode(bot->irc.session, mode);
}

/*int sploosh_irc_cmd_nick(const char *newnick) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	return irc_cmd_nick(bot->irc.session, newnick);
}*/

int sploosh_irc_cmd_whois(const char *nick) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	return irc_cmd_whois(bot->irc.session, nick);
}

int sploosh_irc_cmd_msg(const char *nch, const char *text) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	return irc_cmd_msg(bot->irc.session, nch, text);
}

int sploosh_irc_cmd_me(const char *nch, const char *text) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	return irc_cmd_me(bot->irc.session, nch, text);
}

int sploosh_irc_cmd_notice(const char *nch, const char *text) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	return irc_cmd_notice(bot->irc.session, nch, text);
}

int sploosh_irc_cmd_kick(const char *nick, const char *channel, const char *reason) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	return irc_cmd_kick(bot->irc.session, nick, channel, reason);
}

int sploosh_irc_cmd_ctcp_request(const char *nick, const char *request) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	return irc_cmd_ctcp_request(bot->irc.session, nick, request);
}

int sploosh_irc_cmd_ctcp_reply(const char *nick, const char *reply) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	return irc_cmd_ctcp_request(bot->irc.session, nick, reply);
}

sploosh_error_t sploosh_irc_run(void) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	assert(bot->irc.session == NULL);

	irc_callbacks_t callbacks = {
		&sploosh_irc_event_connect,
		&sploosh_irc_event_nick,
		&sploosh_irc_event_quit,
		&sploosh_irc_event_join,
		&sploosh_irc_event_part,
		&sploosh_irc_event_mode,
		&sploosh_irc_event_umode,
		&sploosh_irc_event_topic,
		&sploosh_irc_event_kick,
		&sploosh_irc_event_channel,
		&sploosh_irc_event_privmsg,
		&sploosh_irc_event_notice,
		&sploosh_irc_event_invite,
		&sploosh_irc_event_ctcp_req,
		&sploosh_irc_event_ctcp_rep,
		&sploosh_irc_event_ctcp_action,
		&sploosh_irc_event_unknown,
		&sploosh_irc_event_numeric,
		&sploosh_irc_event_dcc_chat_req,
		&sploosh_irc_event_dcc_send_req
	};

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
		sploosh_log_eprintf(SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Couldn't connect to %s: %s.", bot->info.server, irc_strerror(irc_errno(bot->irc.session)));

		irc_destroy_session(bot->irc.session);
		bot->irc.session = NULL;

		return SPLOOSH_IRC_COULDNTCONNECT;
	}

	sploosh_log_puts(SPLOOSH_LOG_NOTICE, "Connecting...");

	if(irc_run(bot->irc.session) != LIBIRC_ERR_OK) {
		int error = irc_errno(bot->irc.session);
		if(error != LIBIRC_ERR_CLOSED) {
			sploosh_log_eprintf(SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "IRC error %i: %s.", error, irc_strerror(error));

			irc_destroy_session(bot->irc.session);
			bot->irc.session = NULL;

			return SPLOOSH_IRC_GENERICERROR;
		}
	}

	sploosh_log_puts(SPLOOSH_LOG_NOTICE, "Disconnected from the server.");

	irc_destroy_session(bot->irc.session);
	bot->irc.session = NULL;

	return SPLOOSH_NO_ERROR;
}
