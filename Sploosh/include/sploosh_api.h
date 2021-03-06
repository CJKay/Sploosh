#ifndef SPLOOSH_API_H
#define SPLOOSH_API_H

#include "sploosh_error.h"
#include "sploosh_log.h"
#include "sploosh_config.h"

#include <stdarg.h>
#include <stddef.h>

typedef unsigned int sploosh_pluginid_t;
typedef unsigned int sploosh_dccid_t;
typedef void (*sploosh_event_generic_t)(const char *event, const char *origin, const char **params, unsigned int count);
typedef void (*sploosh_event_numeric_t)(unsigned int event, const char *origin, const char **params, unsigned int count);
typedef void (*sploosh_event_dcc_chat_req_t)(const char *nick, const char *addr, sploosh_dccid_t dccid);
typedef void (*sploosh_event_dcc_send_req_t)(const char *nick, const char *addr, const char *filename, unsigned long size, sploosh_dccid_t dccid);
typedef void (*sploosh_event_command_t)(const char *nch, const char *origin, const char *cmd, const char **params, unsigned int count);

typedef struct sploosh_api {
	struct {
		const char *nickname;
		const char *username;
		const char *realname;
		const char *password;
		const char *server;
		unsigned int port;
	} info;

	struct {
		int (* const quit)(const char *reason);
		int (* const join)(const char *channel, const char *key);
		int (* const part)(const char *channel);
		int (* const invite)(const char *nick, const char *channel);
		int (* const names)(const char *channel);
		int (* const list)(const char *channel);
		int (* const topic)(const char *channel, const char *topic);
		int (* const channel_mode)(const char *channel, const char *mode);
		int (* const user_mode)(const char *mode);
		/*int (* const nick)(const char *newnick);*/
		int (* const whois)(const char *nick);
		int (* const msg)(const char *nch, const char *text);
		int (* const me)(const char *nch, const char *text);
		int (* const notice)(const char *nch, const char *text);
		int (* const kick)(const char *nick, const char *channel, const char *reason);
		int (* const ctcp_request)(const char *nick, const char *request);
		int (* const ctcp_reply)(const char *nick, const char *reply);
	} commands;

	struct {
		sploosh_error_t (* const printf)(sploosh_log_statement_t stmt, const char *format, ...);
		sploosh_error_t (* const puts)(sploosh_log_statement_t stmt, const char *str);
		sploosh_error_t (* const eprintf)(sploosh_log_statement_t stmt, unsigned int line, const char *file, const char *format, ...);
		sploosh_error_t (* const eputs)(sploosh_log_statement_t stmt, unsigned int line, const char *file, const char *str);
		void (* const setstmt)(const char *stmt);
	} log;

	struct {
		int (* const snprintf)(char *str, size_t str_m, const char *fmt, ...);
		int (* const vsnprintf)(char *str, size_t str_m, const char *fmt, va_list ap);
		int (* const asprintf)(char **ptr, const char *fmt, ...);
		int (* const vasprintf)(char **ptr, const char *fmt, va_list ap);
		int (* const strbeginsw)(const char *str, const char *substr);
		int (* const strendsw)(const char *str, const char *substr);
	} utils;

	struct {
		sploosh_event_generic_t connect;
		sploosh_event_generic_t nick;
		sploosh_event_generic_t quit;
		sploosh_event_generic_t join;
		sploosh_event_generic_t part;
		sploosh_event_generic_t mode;
		sploosh_event_generic_t umode;
		sploosh_event_generic_t topic;
		sploosh_event_generic_t kick;
		sploosh_event_generic_t channel;
		sploosh_event_generic_t privmsg;
		sploosh_event_generic_t notice;
		sploosh_event_generic_t invite;
		sploosh_event_generic_t ctcp_req;
		sploosh_event_generic_t ctcp_rep;
		sploosh_event_generic_t ctcp_action;
		sploosh_event_generic_t unknown;
		sploosh_event_numeric_t numeric;
		sploosh_event_dcc_chat_req_t dcc_chat_req;
		sploosh_event_dcc_send_req_t dcc_send_req;
		sploosh_event_command_t command;
	} events;

	sploosh_plugin_settings_t settings;
} sploosh_api_t;

#endif /* SPLOOSH_API_H */
