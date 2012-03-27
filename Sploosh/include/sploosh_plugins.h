#ifndef SPLOOSH_PLUGINS_H
#define SPLOOSH_PLUGINS_H

#ifdef SPLOOSH_EXECUTABLE

#include "sploosh_defs.h"
#include "sploosh_error.h"

#include <libmod/libmod.h>

typedef struct sploosh_plugins {
	unsigned int count;
	libmod_module_t *plugin[SPLOOSH_MAXPLUGINS];
} sploosh_plugins_t;

sploosh_error_t sploosh_plugins_add(libmod_module_t *plugin);
sploosh_error_t sploosh_plugins_remove(libmod_module_t *plugin);
void sploosh_plugins_clear(void);

#endif

#endif /* SPLOOSH_PLUGINS_H */
