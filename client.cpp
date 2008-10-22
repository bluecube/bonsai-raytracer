#include <stdio.h>
#include <unistd.h>

const unsigned long DEFAULT_PORT = 91823;
const unsigned int DEFAULT_GRANULARITY = 8;

void usage(char *name){
	printf("Usage: %s [OPTIONS] [FILE]\n", name);
	printf("Render a scene from FILE.\n\n");

	printf("\t-s, --server=SERVER[:PORT][,SERVER[:PORT]...]\n");
	printf("\t\tRendering server(s) to connect to.\n");
	printf("\t\tIf port is ommited use the default %i.\n", DEFAULT_PORT);
	printf("\t-g, --granularity=NUMBER\n");
	printf("\t\tGranularity of the rendering -- how many chunks does each\n")
	printf("\t\tthread of a rendering server have to render on average. Default %i.", DEFAULT_GRANULARITY);
	printf("\t-r, --local-renderer[=PORT]\n");
	printf("\t\tLaunch a local rendering server.\n");
	printf("\t\tIf port is ommited use the default %i.\n", DEFAULT_PORT);
	printf("\t-h, --help\n");
	printf("\t\tThis help.\n");
}

int main(int argc, char **argv){
	int opt;

	printf("Bonsai rayracer client\n");

	while(1){
		int optionIndex = 0
		static struct option[] = {
			{"server", 1, 0, 's'},
			{"granularity", 1, 0, 'g'},
			{"local-renderer", 2, 0, 'r'},
			{"help", 0, 0, 'h'},
			{0, 0, 0, 0},
		} longopts;
		opt = getopt_long(argc, argv, "s:r::g:h", longopts, &optionIndex);

		if(opt == -1)
			break;

		switch(opt){
		case 's':
			
		case 'h':
			usage(argv[0]);
			return 0

		}
	}
}
