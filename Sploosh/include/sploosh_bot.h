#ifndef SPLOOSH_BOT_H
#define SPLOOSH_BOT_H

#include "sploosh_botinfo.h"
#include "sploosh_plugins.h"

#include "sploosh_log.h"
#include "sploosh_error.h"
#include "sploosh_irc.h"

#include <libconfig.h>

typedef struct sploosh_bot {
	sploosh_botinfo_t info;
	sploosh_plugins_t plugins;
	sploosh_log_t log;
	sploosh_ircinfo_t irc;
	config_t cfg;
} sploosh_bot_t;

#endif /* SPLOOSH_BOT_H */
