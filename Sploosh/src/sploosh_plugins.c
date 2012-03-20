#include "../include/sploosh_plugins.h"

#include <stddef.h>
#include <stdbool.h>

sploosh_error_t sploosh_plugins_add(sploosh_plugins_t *list, libmod_module_t *plugin) {
	bool freespace = false;
	uint64_t index = 0;
	for(uint64_t i = 0; i < SPLOOSH_MAXPLUGINS; i++) {
		if(list->plugin[i] == plugin) {
			return SPLOOSH_PLUGINS_ALREADYLOADED;
		} else if(list->plugin[i] == NULL) {
			index = i;
			freespace = true;

			break;
		}
	}

	if(freespace) {
		list->plugin[index] = plugin;
		list->count++;

		return SPLOOSH_NO_ERROR;
	} else {
		return SPLOOSH_PLUGINS_MAXREACHED;
	}
}

sploosh_error_t sploosh_plugins_remove(sploosh_plugins_t *list, libmod_module_t *plugin) {
	for(uint64_t i = 0; i < list->count; i++) {
		if(list->plugin[i] == plugin) {
			list->plugin[i] = NULL;
			return SPLOOSH_NO_ERROR;
		}
	}

	return SPLOOSH_PLUGINS_NOTINLIST;
}
