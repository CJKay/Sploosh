#ifndef SPLOOSH_CONFIG_H
#define SPLOOSH_CONFIG_H

#include "sploosh_error.h"
#include "sploosh_bot.h"
#include "sploosh_defs.h"

typedef struct sploosh_plugin_setting {
	const char *name;

	union {
		long ival;
		long long llval;
		double fval;
		char *sval;
	};
} sploosh_plugin_setting_t;

typedef struct sploosh_plugin_settings {
	unsigned int count;
	sploosh_plugin_setting_t settings[SPLOOSH_PLUGIN_MAXSETTINGS];
} sploosh_plugin_settings_t;

sploosh_error_t sploosh_config_import(const char *cfgfile);
void sploosh_config_destroy(void);

#endif
