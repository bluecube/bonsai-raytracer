/**
 * \file
 * Client entry point and vase of the network protocol.
 */
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include "net_json.h"
#include "object.h"
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
 * Send the hello message imediately after connecting.
 */
void send_hello(struct net_json *connection, const char *hostname){
	json_object *message = json_object_new_array();
	json_object_array_add(message, json_object_new_string("Hello"));
	json_object_array_add(message, json_object_new_string(hostname));
	
	json_object *version = json_object_new_array();
	json_object_array_add(version, json_object_new_int(CLIENT_VERSION_MAJOR));
	json_object_array_add(version, json_object_new_int(CLIENT_VERSION_MINOR));
	json_object_array_add(version, json_object_new_int(CLIENT_VERSION_REV));

	json_object_array_add(message, version);
	
	net_json_write(connection, message);
}

/**
 * Read commands from input and decide what to do with them.
 */
void work(struct net_json *connection){
	bool keepRunning = true;
	while(keepRunning){
		json_object *obj = net_json_read(connection);

		json_object *typeObj = json_object_object_get(obj, "type");
		if(typeObj == NULL){
			error(0, "Protocol error (message type not specified).");
		}

		const char *type = json_object_get_string(typeObj);

		if(!strcmp(type, "scene")){
			printf("Would load scene\n");
		}else if(!strcmp(type, "chunk")){
			printf("Would process scene\n");
		}else if(!strcmp(type, "finished")){
			keepRunning = false;
			return;
		}else{
			error(0, "Protocol error (unknow message type \"%s\").", type);
		}

		json_object_put(obj);
	}
}

int main(int argc, char **argv){
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
	send_hello(connection, hostname);

	work(connection);

	net_json_close(connection);
	
	return EXIT_SUCCESS;
}

