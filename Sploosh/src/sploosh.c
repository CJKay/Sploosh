#include <libmod/libmod.h>

#include <stddef.h>

libmod_application_t libmod_application = {
		{
				"Sploosh",
				NULL,
				{0, 0, 1, 0},
				{ 0,
						{ }
				}
		}
};

int main(int argc, char *argv[]) {
	if(argc < 2 || strcmp(argv[1], "help") == 0) {
		puts("Usage: sploosh <bot name>");
		puts("    Runs the Sploosh bot located in `./bots/<bot name>`");

		return 1;
	}

	return 0;
}
