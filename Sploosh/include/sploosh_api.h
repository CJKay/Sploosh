#ifndef SPLOOSH_API_H

#define SPLOOSH_API_H

typedef struct sploosh_api {
	struct {
		const char *nickname;
		const char *username;
		const char *realname;
		const char *password;
		const char *server;
		uint16_t port;
	} info;

	struct {
		int (*join)(const char *channel, const char *key);
		int (*part)(const char *channel);
		int (*invite)(const char *nick, const char *channel);
		int (*names)(const char *channel);
		int (*list)(const char *channel);
		int (*topic)(const char *channel, const char *topic);
		int (*channel_mode)(const char *channel, const char *mode);
		int (*user_mode)(const char *mode);
		int (*kick)(const char *nick, const char *channel, const char *reason);
	} commands;
} sploosh_api_t;

#endif /* SPLOOSH_API_H */
