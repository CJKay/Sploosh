#ifndef SPLOOSH_H
#define SPLOOSH_H

#include <libmod/libmod.h>

#include <stddef.h>

libmod_application_t libmod_application = {
		{
				"Sploosh",
				NULL,
				NULL, /* Context stub */
				{ 0, 0, 1, 0 },
				{ 0,
						{ }
				}
		}
};

void sploosh_printusage(void);
void sploosh_printversion(void);

#endif // SPLOOSH_H
