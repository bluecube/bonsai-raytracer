#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <list>

#include "common/Scene.h"
#include "client/RenderDispatcher.h"
#include "client/RenderServerConnection.h"

const unsigned long DEFAULT_PORT = 91823;
const unsigned int DEFAULT_GRANULARITY = 16;
const unsigned int DEFAULT_W = 1024;

const char *DEFAULT_EXTENSION = ".hdr";

void usage(char *name){
	printf("Usage: %s [OPTIONS] INPUT_FILE\n", name);
	printf("Render a scene from INPUT_FILE.\n\n");

	printf("Processing options\n");
	printf("\t-g, --granularity=NUMBER\n");
	printf("\t\tGranularity of the rendering -- how many chunks does each\n");
	printf("\t\tthread of a rendering server have to render on average.\n");
	printf("\t\tDefault value is %i.\n", DEFAULT_GRANULARITY);
	printf("\t-h, --help\n");
	printf("\t\tThis help.\n");
	printf("\t-o, --output=FILE\n");
	printf("\t\tSave output file at this location. Defaults to INPUT_FILE%s\n", DEFAULT_EXTENSION);
	printf("\t-s, --server=SERVER[:PORT][,SERVER[:PORT]...]\n");
	printf("\t\tRendering server(s) to connect to.\n");
	printf("\t\tIf port is ommited use the default %lu.\n", DEFAULT_PORT);
#if WITH_SDL
	printf("\t-u, --gui\n");
	printf("\t\tEnable SDL gui.\n");
#endif

	printf("\t-r, --resolution=WIDTH\n");
	printf("\t\tOutput resolution. Height is calculated from the aspect ratio in\n");
	printf("\t\tthe input file. Default width is %i.\n", DEFAULT_W);
}

int main(int argc, char **argv){
	char *outputFile = 0;
	bool usedDefaultOutputFile = false;
	unsigned int width = DEFAULT_W;
	unsigned int granularity = DEFAULT_GRANULARITY;
	bool gui = false;

	RenderDispatcher dispatcher;

	printf("Bonsai rayracer client\n");

	dispatcher.set_granularity(DEFAULT_GRANULARITY);

	while(1){
		int opt;
		char *tmp;

		static struct option longopts[] = {
			{"granularity", 1, 0, 'g'},
			{"help", 0, 0, 'h'},
			{"output", 1, 0, 'o'},
			{"resolution", 1, 0, 'r'},
			{"server", 1, 0, 's'},
#if WITH_SDL
			{"gui", 1, 0, 'u'},
#endif
			{0, 0, 0, 0}
		};
		opt = getopt_long(argc, argv, "g:ho:r:s:", longopts, 0);

		if(opt == -1)
			break;

		switch(opt){
		case 's':
			tmp = optarg;
			while(*optarg){
				if(*optarg == ','){
					*optarg = '\0';
					dispatcher.add_connection(new RenderServerConnection(tmp));
					tmp = optarg + 1;
				}
				++optarg;
			}
			dispatcher.add_connection(new RenderServerConnection(tmp));
			break;
		case 'g':
			dispatcher.set_granularity(atoi(optarg));
			break;
		case 'h':
			usage(argv[0]);
			return 0;
		case 'o':
			outputFile = optarg;
			break;
		case 'r':
			width = atoi(optarg);
			break;
#if WITH_SDL
		case 'u':
			gui = true;
			break;
#endif
		}
	}

	if(!dispatcher.get_available_threads()){
		printf("We need at least one render server and one render thread.\n");
		return 1;
	}

	if(optind >= argc){
		printf("An input file must be specified.\n");
		usage(argv[0]);
		return 1;
	}

	if(optind < argc - 1){
		printf("Only the first input file will be processed.\n");
	}

	if(!outputFile){
		outputFile = new char[strlen(argv[optind]) + strlen(DEFAULT_EXTENSION) + 1];
		sprintf(outputFile, "%s%s", argv[optind], DEFAULT_EXTENSION);
		usedDefaultOutputFile = true;
	}

	Scene scene;
	//loadScene(&scene, argv[optind]);
	unsigned height = width / scene.get_aspect_ratio();

	Pixmap outputPixmap(width, height);

	dispatcher.set_scene(&scene);
	dispatcher.set_pixmap(&outputPixmap);

	printf("Starting rendering\n");
	dispatcher.go();

	while(!dispatcher.is_finished()){
		dispatcher.wait_for_update();
		printf("%.2f %%; chunks: %u waiting, %u working\n",
		       100 * dispatcher.get_progress(),
		       dispatcher.get_waiting_chunks(),
		       dispatcher.get_working_chunks());
	}

	if(usedDefaultOutputFile)
		delete outputFile;
	
	return 0;
}