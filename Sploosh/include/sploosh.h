#ifndef SPLOOSH_H
#define SPLOOSH_H

#ifndef SPLOOSH_EXECUTABLE

#include "sploosh_api.h"

#include <libmod/libmod.h>

#define SPLOOSH_MODULE(name, description, vmaj, vmin, vrev, vbld) \
	void sploosh_main(void);\
	\
	libmod_module_t libmod_module = {\
		{\
			name,\
			description,\
			NULL,\
			{ vmaj, vmin, vrev, vbld },\
			{ 1, { { "sploosh_main", &sploosh_main } } }\
		}\
	};\
	\
	void sploosh_main(void)\

#define sploosh_api ((sploosh_api_t *)libmod_module.appcontext)

#endif

#endif /* SPLOOSH_H */
