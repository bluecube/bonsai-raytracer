/**
 * \bug I'm using struct printbuf from json-c which is not normally exported.
 * This is just a quick and dirty solution that must be fixed later!
 */

#include "net_json.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <printbuf.h>

#include <stdio.h>

#include "util.h"

struct net_json{
	int fd;

	struct printbuf *buf;
	struct json_tokener *tok;
};

/**
 * Read a line from a socket to a print buffer buf.
 * Calls error() in case of any problems.
 */
static void read_line(int fd, struct printbuf *buf){
	printbuf_reset(buf);

	char c;
	do{
		ssize_t status = read(fd, &c, 1);
		
		if(status == -1){
			error(errno, NULL);
		}else if(status < 1){
			error(0, "Remote side closed the connection up.");
		}

		if(printbuf_memappend(buf, &c, 1) == -1){
			error(errno, NULL);
		}
	}while(c != '\n');
}

/**
 * Allocate a new empty connection.
 * Calls error() in case of any problems.
 */
static struct net_json *net_json_new(){
	struct net_json *connection;

	connection = checked_malloc(sizeof(*connection));

	errno = 0;
	connection->buf = printbuf_new();
	if(!connection->buf){
		error(errno, NULL);
	}
	
	errno = 0;
	connection->tok = json_tokener_new();
	if(!connection->tok){
		error(errno, NULL);
	}

	return connection;
}

/**
 * Free the connection structures without closing the connection.
 */
static void net_json_destroy(struct net_json *connection){
	printbuf_free(connection->buf);
	json_tokener_free(connection->tok);
	free(connection);
}

/**
 * Open a connection and return the corresponding net_json structure.
 * Calls error() in case of any problems.
 * \note The complicated parts of this function are taken from an example
 * in getaddrinfo manpage.
 * \param host Host address (IP or hostname).
 * \param service Host service (service name or ASCII port number).
 */
struct net_json *net_json_connect(const char *host, const char *service){
	struct net_json *connection = net_json_new();

	struct addrinfo hints;
	struct addrinfo *result, *rp;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC; /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;

	int code = getaddrinfo(host, service, &hints, &result);
	if(code != 0){
		error(0, gai_strerror(code));
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
		error(0, "Could not connect.");
	}
	
	connection->fd = sfd;

	return connection;
}

void net_json_close(struct net_json *connection){
	close(connection->fd);
	net_json_destroy(connection);
}

/**
 * Write a JSON object to the connection and decrease its
 * reference count.
 * Calls error() in case of any problems.
 */
void net_json_write(struct net_json *connection, json_object *message){
	errno = 0;
	const char *message_string = json_object_to_json_string(message);
	if(!message_string){
		error(errno, NULL);
	}

	size_t length = strlen(message_string);
	ssize_t written = write(connection->fd, message_string, length);
	if(written == -1){
		error(errno, NULL);
	}

	written = write(connection->fd, "\n", 1);
	if(written == -1){
		error(errno, NULL);
	}

	json_object_put(message);
}

/**
 * Read a line from the connection and parse it as a json object.
 * The returned object has ref count 1.
 */
json_object *net_json_read(struct net_json *connection){
	read_line(connection->fd, connection->buf);

	json_tokener_reset(connection->tok);

	json_object *obj = json_tokener_parse_ex(connection->tok,
		connection->buf->buf, connection->buf->bpos);
	
	if(obj == NULL){
		if(connection->tok->err != json_tokener_success){
			error(0, "Error in JSON data (%s at offset %d).\n",
				json_tokener_errors[connection->tok->err],
				connection->tok->char_offset);
		}else{
			error(errno, NULL);
		}
	}

	return obj;
}

