#include "../include/sploosh_config.h"
#include "../include/sploosh_error.h"
#include "../include/sploosh_log.h"
#include "../include/sploosh_plugins.h"
#include "../include/sploosh_api.h"
#include "../include/sploosh_utils.h"
#include "../include/sploosh.h"

#include "../include/snprintf.h"

#include <libconfig.h>

#include <string.h>

extern libmod_application_t libmod_application;

static void sploosh_config_loadsettings(config_setting_t *cfg_setting, sploosh_plugin_setting_t *sploosh_setting) {
	sploosh_setting->name = cfg_setting->name;

	switch(cfg_setting->type) {
		case CONFIG_TYPE_INT:
			sploosh_setting->type = SPLOOSH_TYPE_INT;
			sploosh_setting->ival = cfg_setting->value.ival;
			break;
		case CONFIG_TYPE_INT64:
			sploosh_setting->type = SPLOOSH_TYPE_INT64;
			sploosh_setting->llval = cfg_setting->value.llval;
			break;
		case CONFIG_TYPE_FLOAT:
			sploosh_setting->type = SPLOOSH_TYPE_FLOAT;
			sploosh_setting->fval = cfg_setting->value.fval;
			break;
		case CONFIG_TYPE_STRING:
			sploosh_setting->type = SPLOOSH_TYPE_STRING;
			sploosh_setting->sval = cfg_setting->value.sval;
			break;
		case CONFIG_TYPE_BOOL:
			sploosh_setting->type = SPLOOSH_TYPE_BOOL;
			sploosh_setting->ival = cfg_setting->value.ival;
			break;
		case CONFIG_TYPE_LIST:
			sploosh_setting->type = SPLOOSH_TYPE_LIST;
			sploosh_setting->list = calloc(1, sizeof(*sploosh_setting->list));
			sploosh_setting->list->length = cfg_setting->value.list->length;
			sploosh_setting->list->elements = calloc(1, sizeof(sploosh_plugin_setting_t) * cfg_setting->value.list->length);

			int i;
			for(i = 0; i < sploosh_setting->list->length; i++)
				sploosh_config_loadsettings(cfg_setting->value.list->elements[i], &sploosh_setting->list->elements[i]);

			break;
		default:
			sploosh_setting->type = SPLOOSH_TYPE_UNKNOWN;
			sploosh_setting->llval = 0;
			break;
	}
}

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

			module->appcontext = calloc(1, sizeof(sploosh_api_t));
			sploosh_api_t *api = module->appcontext;

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
					&sploosh_irc_cmd_quit,
					&sploosh_irc_cmd_join,
					&sploosh_irc_cmd_part,
					&sploosh_irc_cmd_invite,
					&sploosh_irc_cmd_names,
					&sploosh_irc_cmd_list,
					&sploosh_irc_cmd_topic,
					&sploosh_irc_cmd_channel_mode,
					&sploosh_irc_cmd_user_mode,
					/*&sploosh_irc_cmd_nick,*/
					&sploosh_irc_cmd_whois,
					&sploosh_irc_cmd_msg,
					&sploosh_irc_cmd_me,
					&sploosh_irc_cmd_notice,
					&sploosh_irc_cmd_kick,
					&sploosh_irc_cmd_ctcp_request,
					&sploosh_irc_cmd_ctcp_reply
				},

				{
					&sploosh_log_printf,
					&sploosh_log_puts,
					&sploosh_log_eprintf,
					&sploosh_log_eputs
				},

				{
					&sploosh_utils_snprintf,
					&sploosh_utils_vsnprintf,
					&sploosh_utils_asprintf,
					&sploosh_utils_vasprintf,
					&sploosh_utils_strbeginsw,
					&sploosh_utils_strendsw
				}
			};

			memcpy(api, &api_tmp, sizeof(api_tmp));

			config_setting_t *plugin_node;

			plugin_node = config_lookup(&bot->cfg, module->stub.name);
			if(plugin_node != NULL && plugin_node->type == CONFIG_TYPE_GROUP) {
				int j;
				for(j = 0; (j < SPLOOSH_MAXPLUGINS) && (j < config_setting_length(plugin_node)); ++j) {
					config_setting_t *plugin_settings = config_setting_get_elem(plugin_node, j);
					sploosh_config_loadsettings(plugin_settings, &api->settings.settings[api->settings.count++]);
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
