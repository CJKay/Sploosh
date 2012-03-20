#include "../include/sploosh_config.h"
#include "../include/sploosh_error.h"
#include "../include/sploosh_log.h"

#include <libconfig.h>

sploosh_error_t sploosh_config_import(sploosh_bot_t *bot, const char *cfgfile) {
	config_t cfg;
	//config_setting_t *setting;

	config_init(&cfg);

	if(!config_read_file(&cfg, cfgfile)) {
		sploosh_log_eprintf(&bot->log, SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Configuration error: %s:%d - %s.", cfgfile, config_error_line(&cfg), config_error_text(&cfg));
		config_destroy(&cfg);

		return SPLOOSH_CONFIG_COULDNTLOAD;
	}

	if(!config_lookup_string(&cfg, "nickname", &bot->info.nickname)) {
		sploosh_log_eprintf(&bot->log, SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Configuration error: %s:%d - %s.", cfgfile, config_error_line(&cfg), "Couldn't find nickname value");
		config_destroy(&cfg);

		return SPLOOSH_CONFIG_MISSINGVALUE;
	}

	if(!config_lookup_string(&cfg, "username", &bot->info.username)) {
		sploosh_log_eprintf(&bot->log, SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Configuration error: %s:%d - %s.", cfgfile, config_error_line(&cfg), "Couldn't find username value");
		config_destroy(&cfg);

		return SPLOOSH_CONFIG_MISSINGVALUE;
	}

	if(!config_lookup_string(&cfg, "realname", &bot->info.realname)) {
		sploosh_log_eprintf(&bot->log, SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Configuration error: %s:%d - %s.", cfgfile, config_error_line(&cfg), "Couldn't find realname value");
		config_destroy(&cfg);

		return SPLOOSH_CONFIG_MISSINGVALUE;
	}

	config_lookup_string(&cfg, "password", &bot->info.password);

	if(!config_lookup_string(&cfg, "server", &bot->info.server)) {
		sploosh_log_eprintf(&bot->log, SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Configuration error: %s:%d - %s.", cfgfile, config_error_line(&cfg), "Couldn't find server value");
		config_destroy(&cfg);

		return SPLOOSH_CONFIG_MISSINGVALUE;
	}

	if(!config_lookup_int(&cfg, "port", (long int *)&bot->info.port)) {
		sploosh_log_eprintf(&bot->log, SPLOOSH_LOG_ERROR, __LINE__, __FILE__, "Configuration error: %s:%d - %s.", cfgfile, config_error_line(&cfg), "Couldn't find port value");
		config_destroy(&cfg);

		return SPLOOSH_CONFIG_MISSINGVALUE;
	}

	config_destroy(&cfg);

	return SPLOOSH_NO_ERROR;
}
