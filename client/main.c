#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <json/json.h>

#include "shared_defs.h"
#include "object.h"

#define CLIENT_VERSION \
	#CLIENT_VERSION_MAJOR "." \
	#CLIENT_VERSION_MINOR "." \
	#CLIENT_VERSION_REV

void usage(const char *argv0){
	printf("Usage:\n\n");

	printf("%s host [port]\n", argv0);
}

/**
 * Resolve address, create a socket, connect and return it.
 * Calls exit() if there is a problem.
 * \param addr address in format host[:service]
 * \note This is taken from an example in getaddrinfo manpage.
 */
int create_socket(const char *host, const char *port){
	struct addrinfo hints;
	struct addrinfo *result, *rp;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC; /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	int code = getaddrinfo(host, port, &hints, &result);
	if(code != 0){
		fprintf(stderr, "getaddrinfo() failed: %s\n", gai_strerror(code));
		exit(EXIT_FAILURE);
	}
	/* Try connecting to all of the returned addresses until
	 * one connection succeeds. */
	int sfd;
	for(rp = result; rp != NULL; rp = rp->ai_next){
		sfd =  socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if(sfd == -1){
			continue;
		}

		if(connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1){
			break;
		}

		close(sfd);
	}

	freeaddrinfo(result);

	if(rp == NULL){
		fprintf(stderr, "Could not connect\n");
		exit(EXIT_FAILURE);
	}

	return sfd;
}

int main(int argc, char **argv){
	if(argc < 2 || argc > 3){
		usage(argv[0]);
		return EXIT_FAILURE;
	}

	int sfd = create_socket(argv[1], argc >= 3 ? argv[2] : "23232");
	
	write(sfd, "Hello world\n", 12);

	close(sfd);
}
