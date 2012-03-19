#ifndef SPLOOSH_BOT_H
#define SPLOOSH_BOT_H

#include "sploosh_botinfo.h"
#include "sploosh_plugins.h"

typedef struct sploosh_bot {
	sploosh_botinfo_t info;
	sploosh_plugins_t plugins;
} sploosh_bot_t;

#endif // SPLOOSH_BOT_H
