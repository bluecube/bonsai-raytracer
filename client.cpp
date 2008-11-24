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

class RenderServer{
public:
	RenderServer(char *addr){
		printf("server %s\n", addr);
	}

	RenderServer *next;
};

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

	LList<RenderServer> servers;

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
					servers.append_single(new RenderServer(tmp));
					tmp = optarg + 1;
				}
				++optarg;
			}
			servers.append_single(new RenderServer(tmp));
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

	unsigned int threadCount = 0;
	unsigned int serverCount = 0;
	
	RenderServer *rs = servers;
	while(rs){
		if(!(rs -> connect())){
			rs = rs -> next;
			break;
		}

		threadCount += rs -> get_thread_count();
		++serverCount;

		rs = rs -> next;
	}

	if(!serverCount){
		printf("We need at least one render server.\n");
		servers.delete_list();
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

	Scene scene(argv[optind]);
	unsigned int height = width / scene.get_aspect_ratio();

	unsigned int chunksX = 1;
	unsigned int chunksY = 1;

	if(width > height){
		chunksX = chunks * granularity;
	}else{
		chunksY = chunks * granularity;
	}

	

	if(usedDefaultOutputFile)
		delete outputFile;
	
	servers.delete_list();
	return 0;
}
