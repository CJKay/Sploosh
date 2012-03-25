#ifndef SPLOOSH_IRC_H
#define SPLOOSH_IRC_H

#include "sploosh_error.h"

#include <libircclient/libircclient.h>

typedef struct sploosh_ircinfo_t {
	irc_session_t *session;
} sploosh_ircinfo_t;

sploosh_error_t sploosh_irc_run(void);

typedef irc_dcc_t sploosh_irc_dcc_t;
typedef void (*sploosh_irc_event_callback_t)(const char *event, const char *origin, const char **params, unsigned int count);
typedef void (*sploosh_irc_eventcode_callback_t)(unsigned int event, const char *origin, const char **params, unsigned int count);
typedef void (*sploosh_irc_event_dcc_chat_t)(const char *nick, const char *addr, sploosh_irc_dcc_t dccid);
typedef void (*sploosh_irc_event_dcc_send_t)(const char *nick, const char *addr, const char *filename, unsigned long size, sploosh_irc_dcc_t dccid);

int sploosh_irc_cmd_join(const char *channel, const char *key);
int sploosh_irc_cmd_part(const char *channel);
int sploosh_irc_cmd_invite(const char *nick, const char *channel);
int sploosh_irc_cmd_names(const char *channel);
int sploosh_irc_cmd_list(const char *channel);
int sploosh_irc_cmd_topic(const char *channel, const char *topic);
int sploosh_irc_cmd_channel_mode(const char *channel, const char *mode);
int sploosh_irc_cmd_user_mode(const char *mode);
int sploosh_irc_cmd_kick(const char *nick, const char *channel, const char *reason);

#endif /* SPLOOSH_IRC_H */
