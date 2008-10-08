#include <stdio.h>
#include <unistd.h>

void usage(char *name){
	printf("Usage: %s [OPTION] [FILE]\n", name);
	printf("Render a scene from FILE.\n\n");

	printf("\t-s SERVER[:PORT][,SERVER[:PORT]...]\tRendering server(s) to connect to.\n");
	printf("\t\t\tIf port is ommited use the default %i.\n", PORT);
	printf("\t-g NUMBER\tGranularity of the rendering -- how many chunks does each\n")
	printf("\t\t\tthread of a rendering server have to render on average. Default 8.");
	printf("\t-r [PORT]\t\tLaunch a local rendering server.\n");
	printf("\t-h\t\tThis help.\n");
}

int main(int argc, char **argv){
	int opt;

	printf("Bonsai rayracer client\n");

	while((opt = getopt(argc, argv, "s:r::g:h")) != -1){
		switch(opt){
		case 's':
			
		case 'h':
			usage(argv[0]);
			return 0

		}
	}
}
