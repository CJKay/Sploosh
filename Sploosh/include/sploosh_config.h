#ifndef SPLOOSH_CONFIG_H
#define SPLOOSH_CONFIG_H

#include "sploosh_defs.h"

typedef enum sploosh_plugin_setting_type {
	SPLOOSH_TYPE_INT = 0,
	SPLOOSH_TYPE_INT64,
	SPLOOSH_TYPE_FLOAT,
	SPLOOSH_TYPE_STRING,
	SPLOOSH_TYPE_BOOL,
	SPLOOSH_TYPE_LIST,
	SPLOOSH_TYPE_UNKNOWN
} sploosh_plugin_setting_type_t;

typedef struct sploosh_plugin_setting_list sploosh_plugin_setting_list_t;

typedef struct sploosh_plugin_setting {
	const char *name;
	sploosh_plugin_setting_type_t type;

	union {
		long ival;
		long long llval;
		double fval;
		char *sval;
		sploosh_plugin_setting_list_t *list;
	};
} sploosh_plugin_setting_t;

typedef struct sploosh_plugin_setting_list {
	unsigned int length;
	sploosh_plugin_setting_t *elements;
} sploosh_plugin_setting_list_t;

typedef struct sploosh_plugin_settings {
	unsigned int count;
	sploosh_plugin_setting_t settings[SPLOOSH_PLUGIN_MAXSETTINGS];
} sploosh_plugin_settings_t;

#ifdef SPLOOSH_EXECUTABLE

#include "sploosh_error.h"
#include "sploosh_bot.h"
#include "sploosh_defs.h"

sploosh_error_t sploosh_config_import(const char *cfgfile);
void sploosh_config_destroy(void);

#endif

#endif
