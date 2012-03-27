#ifndef SPLOOSH_BOTINFO_H
#define SPLOOSH_BOTINFO_H

#ifdef SPLOOSH_EXECUTABLE

#include <stdint.h>

typedef struct sploosh_botinfo {
	const char *nickname;
	const char *username;
	const char *realname;
	const char *password;
	const char *server;
	uint16_t port;
} sploosh_botinfo_t;

#endif

#endif /* SPLOOSH_BOTINFO_H */
