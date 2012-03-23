#ifndef SPLOOSH_IRC_H
#define SPLOOSH_IRC_H

#include "sploosh_error.h"

#include <libircclient/libircclient.h>

typedef struct sploosh_ircinfo_t {
	irc_session_t *session;
} sploosh_ircinfo_t;

sploosh_error_t sploosh_irc_run(void);

#endif /* SPLOOSH_IRC_H */
