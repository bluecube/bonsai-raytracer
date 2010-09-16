/**
 * \file
 * Client entry point and base of the network protocol.
 */

#define _XOPEN_SOURCE 500

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include "color.h"
#include "net_json.h"
#include "object.h"
#include "protocol.h"
#include "random.h"
#include "renderer.h"
#include "scene.h"
#include "shared_defs.h"
#include "util.h"

#define BUFFER_LEN 64

void usage(const char *argv0){
	printf("Usage:\n\n");

	printf("%s --help\n", argv0);
	printf("\tPrint this message and exit succesfully.\n");
	printf("%s host [port]\n", argv0);
	printf("\tConnect to the server and start rendering.\n");
}

/**
 * The actual work loop and the part of a protocol that contains the actions.
 */
void work(struct net_json *connection){
	bool keepRunning = true;

	struct scene s;
	scene_init(&s);

	while(keepRunning){
		json_object *json = net_json_read(connection);
		enum protocol_msg_types type;

		type = protocol_load_msg_type(json);

		switch(type){
		case MSG_TYPE_SCENE:
			scene_empty(&s);
			protocol_load_scene(json, &s);

printf("Scene loaded.\n");
			break;
		case MSG_TYPE_CHUNK:
			(void)0; // declaration cannot have a label, but (void)0 can
			struct renderer_chunk chunk;
			protocol_load_chunk(json, &chunk);

			size_t size = chunk.height * s.width;
			struct color *pixmap = checked_malloc(size * sizeof(struct color));

printf("Rendering chunk.\n");
			renderer_render(&s, &chunk, pixmap);

			struct json_object *jsonPixmap = protocol_store_pixmap(pixmap, size);
			free(pixmap);
			net_json_write(connection, jsonPixmap);

			break;
		case MSG_TYPE_FINISHED:
printf("Finishing\n");
			keepRunning = false;
			break;
		}

		json_object_put(json);
	}
}

int main(int argc, char **argv){
	init_random();

	printf("Bonsai raytracer client v%d.%d.%d\n\n",
		CLIENT_VERSION_MAJOR,
		CLIENT_VERSION_MINOR,
		CLIENT_VERSION_REV);

	if(argc < 2 || argc > 3){
		usage(argv[0]);
		return EXIT_FAILURE;
	}

	if(
		argv[1][0] == '-' &&
		argv[1][1] == '-' &&
		argv[1][2] == 'h' &&
		argv[1][3] == 'e' &&
		argv[1][4] == 'l' &&
		argv[1][5] == 'p'){

		usage(argv[0]);
		return EXIT_SUCCESS;
	}
		
	
	struct net_json *connection;
	if(argc >= 3){
		connection = net_json_connect(argv[1], argv[2]);
	}else{
		char defaultPort[BUFFER_LEN];
		snprintf(defaultPort, BUFFER_LEN, "%d", DEFAULT_PORT);
		connection = net_json_connect(argv[1], defaultPort);
	}
	
	
	char hostname[BUFFER_LEN];
	if(gethostname(hostname, BUFFER_LEN)){
		error(errno, NULL);
	}
	net_json_write(connection, protocol_store_hello(hostname));

	work(connection);

	net_json_close(connection);
	
	return EXIT_SUCCESS;
}

