#include "../include/sploosh_config.h"
#include "../include/sploosh_error.h"
#include "../include/sploosh_log.h"
#include "../include/sploosh.h"

#include <libconfig.h>

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

			libmod_module_t *module;
			if((module = libmod_module_load(&libmod_application, filename)) == NULL) {
				sploosh_log_eprintf(SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Failed to load %s: %s.", filename, libmod_error_string(libmod_error_number()), libmod_error_details());

				config_destroy(&bot->cfg);
				break;
			}
		}
	}

	return SPLOOSH_NO_ERROR;
}

void sploosh_config_destroy(void) {
	sploosh_bot_t *bot = libmod_application.stub.context;

	config_destroy(&bot->cfg);
}
