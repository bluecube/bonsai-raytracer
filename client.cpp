#include "Scene.h"

#include "LList.h"

#include <getopt.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const unsigned long DEFAULT_PORT = 91823;
const unsigned int DEFAULT_GRANULARITY = 8;
const unsigned int DEFAULT_W = 1024;

const char *DEFAULT_EXTENSION = ".hdr";

/// A class representing a connection to the render server.
class RenderServerConnection{
public:
	RenderServerConnection(char *addr){
		printf("server %s\n", addr);
	}

	unsigned getAvailableThreadCount(){
		return 1;
	}
};

class Chunk{
	public:
	double finished; /// < What part of this chunk is finished.

	std::list<RenderServerConnection>::const_iterator assignedTo;
	unsigned chunkX, chunkY;
}

void usage(char *name){
	printf("Usage: %s [OPTIONS] INPUT_FILE\n", name);
	printf("Render a scene from FILE.\n\n");

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
	printf("\t\tIf port is ommited use the default %i.\n", DEFAULT_PORT);

	printf("\t-r, --resolution=WIDTH\n");
	printf("\t\tOutput resolution. Height is calculated from the aspect ratio in\n");
	printf("\t\tthe input file. Default width is %i.\n", DEFAULT_W);
}

int main(int argc, char **argv){

	char *outputFile = 0;
	bool usedDefaultOutputFile = false;
	unsigned int width = DEFAULT_W;
	unsigned int granularity = DEFAULT_GRANULARITY;

	printf("Bonsai rayracer client\n");

	std::vector<RenderServer> servers;
#include "Scene.h"

#include "LList.h"

#include <getopt.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

const unsigned long DEFAULT_PORT = 91823;
const unsigned int DEFAULT_GRANULARITY = 8;
const unsigned int DEFAULT_W = 1024;

const char *DEFAULT_EXTENSION = ".hdr";

class Chunk{
	public:
	double finished; /// < What part of this chunk is finished.

	std::list<RenderServerConnection>::const_iterator assignedTo;
	unsigned chunkX, chunkY;
}

void usage(char *name){
	printf("Usage: %s [OPTIONS] INPUT_FILE\n", name);
	printf("Render a scene from FILE.\n\n");

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
	printf("\t\tIf port is ommited use the default %i.\n", DEFAULT_PORT);

	printf("\t-r, --resolution=WIDTH\n");
	printf("\t\tOutput resolution. Height is calculated from the aspect ratio in\n");
	printf("\t\tthe input file. Default width is %i.\n", DEFAULT_W);
}

int main(int argc, char **argv){
	char *outputFile = 0;
	bool usedDefaultOutputFile = false;
	unsigned int width = DEFAULT_W;
	unsigned int granularity = DEFAULT_GRANULARITY;

	RenderDispatcher dispatcher;

	printf("Bonsai rayracer client\n");


	while(1){
		int opt;
		char *tmp;

		static struct option longopts[] = {
			{"granularity", 1, 0, 'g'},
			{"help", 0, 0, 'h'},
			{"output", 1, 0, 'o'},
			{"resolution", 1, 0, 'r'},
			{"server", 1, 0, 's'},
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
					dispatcher.addConnection(new RenderServerConnection(tmp));
					tmp = optarg + 1;
				}
				++optarg;
			}
			dispatcher.addConnection(new RenderServerConnection(tmp));
			break;
		case 'g':
			granularity = atoi(optarg);
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
		}
	}

	if(!dispatcher.get_available_threads()){
		printf("We need at least one render server and one render thread.\n");
		return 1;
	}

	if(optind >= argc){
		printf("An input file must be specified.\n");
		usage(argv[0]);
		servers.delete_list();
		return 1;
	}

	if(!outputFile){
		outputFile = new char[strlen(argv[optind]) + strlen(DEFAULT_EXTENSION) + 1];
		sprintf(outputFile, "%s%s", argv[optind], DEFAULT_EXTENSION);
		usedDefaultOutputFile = true;
	}

	granularity *= dispatcher.get_available_threads();

	Scene scene(argv[optind]);
	unsigned height = width / scene.get_aspect_ratio();

	unsigned chunksX = 1 + (int)sqrt(granularity * scene.get_aspect_ratio());
	unsigned chunksY = 1 + granularity / chunksX;
	granularity = chunksX * chunksY;

	dispatcher.set_scene(&scene);
	dispatcher.set_pixmap()
	dispatcher.set_chunks(chunksX, chunksY);

	printf("Starting rendering\n"):
	dispatcher.go();

	while(!dispatcher.finished()){
		sleep(5);
		printf("%.2f%%; %u of %u waiting\n",
		       100 * dispatcher.get_progress(), dispatcher.get_waiting_chunks(), granularity);
	}

	if(usedDefaultOutputFile)
		delete outputFile;
	
	return 0;
}
