#ifndef SPLOOSH_CONFIG_H
#define SPLOOSH_CONFIG_H

#include "sploosh_error.h"
#include "sploosh_bot.h"

sploosh_error_t sploosh_config_import(const char *cfgfile);
sploosh_error_t sploosh_config_destroy(void);

#endif
