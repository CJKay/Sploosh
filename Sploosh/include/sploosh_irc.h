#ifndef SPLOOSH_IRC_H
#define SPLOOSH_IRC_H

#ifdef SPLOOSH_EXECUTABLE

#include "sploosh_error.h"

#include <libircclient/libircclient.h>

typedef struct sploosh_ircinfo_t {
	irc_session_t *session;
} sploosh_ircinfo_t;

sploosh_error_t sploosh_irc_run(void);

int sploosh_irc_cmd_quit(const char *reason);
int sploosh_irc_cmd_join(const char *channel, const char *key);
int sploosh_irc_cmd_part(const char *channel);
int sploosh_irc_cmd_invite(const char *nick, const char *channel);
int sploosh_irc_cmd_names(const char *channel);
int sploosh_irc_cmd_list(const char *channel);
int sploosh_irc_cmd_topic(const char *channel, const char *topic);
int sploosh_irc_cmd_channel_mode(const char *channel, const char *mode);
int sploosh_irc_cmd_user_mode(const char *mode);
/*int sploosh_irc_cmd_nick(const char *newnick)*/
int sploosh_irc_cmd_whois(const char *nick);
int sploosh_irc_cmd_msg(const char *nch, const char *text);
int sploosh_irc_cmd_me(const char *nch, const char *text);
int sploosh_irc_cmd_notice(const char *nch, const char *text);
int sploosh_irc_cmd_kick(const char *nick, const char *channel, const char *reason);
int sploosh_irc_cmd_ctcp_request(const char *nick, const char *request);
int sploosh_irc_cmd_ctcp_reply(const char *nick, const char *reply);

#endif

#endif /* SPLOOSH_IRC_H */
