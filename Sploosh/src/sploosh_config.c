#include "../include/sploosh_config.h"
#include "../include/sploosh_error.h"
#include "../include/sploosh_log.h"
#include "../include/sploosh_plugins.h"
#include "../include/sploosh_api.h"
#include "../include/sploosh.h"

#include <libconfig.h>

#include <string.h>

extern libmod_application_t libmod_application;

sploosh_error_t sploosh_config_import(const char *cfgfile) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	config_init(&bot->cfg);

	if(!config_read_file(&bot->cfg, cfgfile)) {
		sploosh_log_eprintf(SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Configuration error: %s:%d - %s.", cfgfile, config_error_line(&bot->cfg), config_error_text(&bot->cfg));
		config_destroy(&bot->cfg);

		return SPLOOSH_CONFIG_COULDNTLOAD;
	}

	if(!config_lookup_string(&bot->cfg, "nickname", &bot->info.nickname)) {
		sploosh_log_eprintf(SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Configuration error: %s:%d - %s.", cfgfile, config_error_line(&bot->cfg), "Couldn't find nickname value");
		config_destroy(&bot->cfg);

		return SPLOOSH_CONFIG_MISSINGVALUE;
	}

	if(!config_lookup_string(&bot->cfg, "username", &bot->info.username)) {
		sploosh_log_eprintf(SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Configuration error: %s:%d - %s.", cfgfile, config_error_line(&bot->cfg), "Couldn't find username value");
		config_destroy(&bot->cfg);

		return SPLOOSH_CONFIG_MISSINGVALUE;
	}

	if(!config_lookup_string(&bot->cfg, "realname", &bot->info.realname)) {
		sploosh_log_eprintf(SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Configuration error: %s:%d - %s.", cfgfile, config_error_line(&bot->cfg), "Couldn't find realname value");
		config_destroy(&bot->cfg);

		return SPLOOSH_CONFIG_MISSINGVALUE;
	}

	config_lookup_string(&bot->cfg, "password", &bot->info.password);

	if(!config_lookup_string(&bot->cfg, "server", &bot->info.server)) {
		sploosh_log_eprintf(SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Configuration error: %s:%d - %s.", cfgfile, config_error_line(&bot->cfg), "Couldn't find server value");
		config_destroy(&bot->cfg);

		return SPLOOSH_CONFIG_MISSINGVALUE;
	}

	if(!config_lookup_int(&bot->cfg, "port", (long int *)&bot->info.port)) {
		sploosh_log_eprintf(SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Configuration error: %s:%d - %s.", cfgfile, config_error_line(&bot->cfg), "Couldn't find port value");
		config_destroy(&bot->cfg);

		return SPLOOSH_CONFIG_MISSINGVALUE;
	}

	config_setting_t *setting;
	if((setting = config_lookup(&bot->cfg, "plugins")) != NULL) {
		int i;
		for(i = 0; (i < SPLOOSH_MAXPLUGINS) && (i < config_setting_length(setting)); ++i) {
			const char *filename = config_setting_get_string_elem(setting, i);

			if(filename == NULL)
				continue;

			libmod_module_t *module;
			if((module = libmod_module_load(&libmod_application, filename)) == NULL) {
				sploosh_log_eprintf(SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Failed to load %s: %s (%s).", filename, libmod_error_string(libmod_error_number()), libmod_error_details());

				config_destroy(&bot->cfg);
				return SPLOOSH_PLUGINS_NOTFOUND;
			}

			sploosh_error_t error;
			if((error = sploosh_plugins_add(module)) != SPLOOSH_NO_ERROR) {
				sploosh_log_eprintf(SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Couldn't add '%s' to plugins (error %d).", module->stub.name, error);

				return error;
			}

			sploosh_api_t *api = calloc(1, sizeof(*api));
			module->appcontext = api;

			sploosh_api_t api_tmp = {
				{
					bot->info.nickname,
					bot->info.username,
					bot->info.realname,
					bot->info.password,
					bot->info.server,
					bot->info.port
				},

				{
					&sploosh_irc_cmd_join,
					&sploosh_irc_cmd_part,
					&sploosh_irc_cmd_invite,
					&sploosh_irc_cmd_names,
					&sploosh_irc_cmd_list,
					&sploosh_irc_cmd_topic,
					&sploosh_irc_cmd_channel_mode,
					&sploosh_irc_cmd_user_mode,
					&sploosh_irc_cmd_kick
				},

				{
					&sploosh_log_printf,
					&sploosh_log_puts,
					&sploosh_log_eprintf,
					&sploosh_log_eputs
				}
			};

			memcpy(api, &api_tmp, sizeof(api_tmp));

			config_setting_t *plugin_settings;

			plugin_settings = config_lookup(&bot->cfg, module->stub.name);
			if(plugin_settings != NULL) {
				int j;
				for(j = 0; (j < SPLOOSH_MAXPLUGINS) && (j < config_setting_length(plugin_settings)); ++j) {
					config_setting_t *keyval;
					keyval = config_setting_get_elem(plugin_settings, j);

					sploosh_plugin_setting_t val;
					val.name = keyval->name;

					switch(keyval->type) {
						case CONFIG_TYPE_INT:
							val.ival = keyval->value.ival;
							break;
						case CONFIG_TYPE_INT64:
							val.llval = keyval->value.llval;
							break;
						case CONFIG_TYPE_FLOAT:
							val.fval = keyval->value.fval;
							break;
						case CONFIG_TYPE_STRING:
							val.sval = keyval->value.sval;
							break;
						case CONFIG_TYPE_BOOL:
							val.ival = keyval->value.ival;
							break;
						default:
							val.llval = 0;
							break;
					}

					api->settings.settings[api->settings.count++] = val;
				}
			}

			void (*sploosh_main)(void) = libmod_function_fromname(&module->stub, "sploosh_main");
			if(sploosh_main == NULL) {
				sploosh_log_eprintf(SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Module '%s' isn't a valid Sploosh plugin.", module->stub.name);
			}

			sploosh_main();
		}
	}

	return SPLOOSH_NO_ERROR;
}

void sploosh_config_destroy(void) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	config_destroy(&bot->cfg);
}
