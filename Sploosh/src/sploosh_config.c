#include "../include/sploosh_config.h"
#include "../include/sploosh_error.h"
#include "../include/sploosh_log.h"

#include <libconfig.h>

sploosh_error_t sploosh_config_import(sploosh_bot_t *bot, const char *cfgfile) {
	config_init(&bot->cfg);

	if(!config_read_file(&bot->cfg, cfgfile)) {
		sploosh_log_eprintf(&bot->log, SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Configuration error: %s:%d - %s.", cfgfile, config_error_line(&bot->cfg), config_error_text(&bot->cfg));
		config_destroy(&bot->cfg);

		return SPLOOSH_CONFIG_COULDNTLOAD;
	}

	if(!config_lookup_string(&bot->cfg, "nickname", &bot->info.nickname)) {
		sploosh_log_eprintf(&bot->log, SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Configuration error: %s:%d - %s.", cfgfile, config_error_line(&bot->cfg), "Couldn't find nickname value");
		config_destroy(&bot->cfg);

		return SPLOOSH_CONFIG_MISSINGVALUE;
	}

	if(!config_lookup_string(&bot->cfg, "username", &bot->info.username)) {
		sploosh_log_eprintf(&bot->log, SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Configuration error: %s:%d - %s.", cfgfile, config_error_line(&bot->cfg), "Couldn't find username value");
		config_destroy(&bot->cfg);

		return SPLOOSH_CONFIG_MISSINGVALUE;
	}

	if(!config_lookup_string(&bot->cfg, "realname", &bot->info.realname)) {
		sploosh_log_eprintf(&bot->log, SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Configuration error: %s:%d - %s.", cfgfile, config_error_line(&bot->cfg), "Couldn't find realname value");
		config_destroy(&bot->cfg);

		return SPLOOSH_CONFIG_MISSINGVALUE;
	}

	config_lookup_string(&bot->cfg, "password", &bot->info.password);

	if(!config_lookup_string(&bot->cfg, "server", &bot->info.server)) {
		sploosh_log_eprintf(&bot->log, SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Configuration error: %s:%d - %s.", cfgfile, config_error_line(&bot->cfg), "Couldn't find server value");
		config_destroy(&bot->cfg);

		return SPLOOSH_CONFIG_MISSINGVALUE;
	}

	if(!config_lookup_int(&bot->cfg, "port", (long int *)&bot->info.port)) {
		sploosh_log_eprintf(&bot->log, SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Configuration error: %s:%d - %s.", cfgfile, config_error_line(&bot->cfg), "Couldn't find port value");
		config_destroy(&bot->cfg);

		return SPLOOSH_CONFIG_MISSINGVALUE;
	}

	config_setting_t *setting;
	if((setting = config_lookup(&bot->cfg, "plugins")) != NULL) {

	}

	return SPLOOSH_NO_ERROR;
}

sploosh_error_t sploosh_config_destroy(sploosh_bot_t *bot) {
	config_destroy(&bot->cfg);

	return SPLOOSH_NO_ERROR;
}
