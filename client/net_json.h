/** \file
 * Wrapper around network communication.
 * Provides interface for sending and receiving json objects 
 * to/from network.
 */
#ifndef BONSAI_CLIENT_NET_JSON_H
#define BONSAI_CLIENT_NET_JSON_H

#include <json.h>

struct net_json;

struct net_json *net_json_connect(const char *host, const char *service);
void net_json_close(struct net_json *connection);

void net_json_write(struct net_json *connection, json_object *message);
json_object *net_json_read(struct net_json *connection);

#endif
