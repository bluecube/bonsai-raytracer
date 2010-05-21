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
void send_hello(struct net_json *connection){
	char name[BUFFER_LEN];
	if(gethostname(name, BUFFER_LEN)){
		error(errno, NULL);
	}

	json_object *message = json_object_new_array();
	json_object_array_add(message, json_object_new_string("Hello"));
	json_object_array_add(message, json_object_new_string(name));
	
	json_object *version = json_object_new_array();
	json_object_array_add(version, json_object_new_int(CLIENT_VERSION_MAJOR));
	json_object_array_add(version, json_object_new_int(CLIENT_VERSION_MINOR));
	json_object_array_add(version, json_object_new_int(CLIENT_VERSION_REV));

	json_object_array_add(message, version);
	
	net_json_write(connection, message);
}

bool get_job(struct net_json *connection){
printf("getting job");
	json_object *obj = net_json_read(connection);
printf("have job");
	
	puts(json_object_to_json_string(obj));
	json_object_put(obj);

	return false;
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
		

	char defaultPort[BUFFER_LEN];
	snprintf(defaultPort, BUFFER_LEN, "%d", DEFAULT_PORT);

	struct net_json *connection = net_json_connect(argv[1],
		argc >= 3 ? argv[2] : defaultPort);
	
	send_hello(connection);

printf("before getting job\n");
	while(1){
printf("before getting job\n");
		bool status = get_job(connection);

		if(!status){
			break;
		}

		//work(connection);
		//send_output(connection);
	}

	net_json_close(connection);
}

