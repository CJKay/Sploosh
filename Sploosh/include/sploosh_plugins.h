#ifndef SPLOOSH_PLUGINS_H
#define SPLOOSH_PLUGINS_H

#include "sploosh_defs.h"
#include "sploosh_error.h"

typedef struct sploosh_plugins {
	uint64_t count;
	libmod_module_t *plugin[SPLOOSH_MAXPLUGINS];
} sploosh_plugins_t;

sploosh_error_t sploosh_plugins_add(sploosh_plugins_t *list, libmod_module_t *plugin);

#endif // SPLOOSH_PLUGINS_H