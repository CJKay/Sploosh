#ifndef SPLOOSH_BOTINFO_H
#define SPLOOSH_BOTINFO_H

#include <stdint.h>

typedef struct sploosh_botinfo {
	const char *name;
	const char *nickname;
	const char *username;
	const char *realname;
	const char *password;
	const char *server;
	const uint16_t port;
} sploosh_botinfo_t;

#endif // SPLOOSH_BOTINFO_H
