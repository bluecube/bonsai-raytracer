/**
 * \file
 * Most of the hard work for the client-server protocol.
 * However protocol.h and protocol.c don't contain the message sequences
 * (these are in main.c).
 */

#ifndef BONSAI_CLIENT_PROTOCOL_H
#define BONSAI_CLIENT_PROTOCOL_H

#include <json.h>

#include "color.h"
#include "renderer.h"
#include "scene.h"
#include "util.h"

/**
 * Types of incoming messages.
 */
enum protocol_msg_types{
	MSG_TYPE_SCENE,
	MSG_TYPE_CHUNK,
	MSG_TYPE_FINISHED,
};

enum protocol_msg_types protocol_load_msg_type(struct json_object *json);
void protocol_load_chunk(struct json_object *json, struct renderer_chunk *chunk);
void protocol_load_scene(struct json_object *json, struct scene *s);

struct json_object *protocol_store_hello(const char *hostname);
struct json_object *protocol_store_pixmap(const struct color *pixmap, size_t length);


#endif
