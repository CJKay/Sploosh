#include "../include/sploosh_plugins.h"
#include "../include/sploosh_bot.h"
#include "../include/sploosh.h"

#include <stddef.h>
#include <stdbool.h>

#include <libmod/libmod.h>

extern libmod_application_t libmod_application;

sploosh_error_t sploosh_plugins_add(libmod_module_t *plugin) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	bool freespace = false;
	int index = 0, i;
	for(i = 0; i < SPLOOSH_MAXPLUGINS; i++) {
		if(bot->plugins.plugin[i] == plugin) {
			return SPLOOSH_PLUGINS_ALREADYLOADED;
		} else if(bot->plugins.plugin[i] == NULL) {
			index = i;
			freespace = true;

			break;
		}
	}

	if(freespace) {
		bot->plugins.plugin[index] = plugin;
		bot->plugins.count++;

		return SPLOOSH_NO_ERROR;
	} else {
		return SPLOOSH_PLUGINS_MAXREACHED;
	}
}

sploosh_error_t sploosh_plugins_remove(libmod_module_t *plugin) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	unsigned int i;
	for(i = 0; i < bot->plugins.count; i++) {
		if(bot->plugins.plugin[i] == plugin) {
			bot->plugins.plugin[i] = NULL;
			return SPLOOSH_NO_ERROR;
		}
	}

	return SPLOOSH_PLUGINS_NOTINLIST;
}

void sploosh_plugins_clear(void) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	unsigned int i;
	for(i = 0; i < bot->plugins.count; i++) {
		if(bot->plugins.plugin[i] != NULL) {
			free(bot->plugins.plugin[i]->appcontext);
			libmod_module_unload(&libmod_application, bot->plugins.plugin[i]);
		}
	}
}
