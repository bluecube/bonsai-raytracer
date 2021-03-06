#include "protocol.h"

#include <math.h>
#include <string.h>

#include "kd_tree.h"
#include "object.h"
#include "objects/cayley.h"
#include "objects/sphere.h"
#include "objects/plane.h"
#include "shared_defs.h"
#include "surface.h"
#include "surfaces/lambertian.h"
#include "light.h"
#include "lights/difuse_white_light.h"
#include "transform.h"


/**
 * Print a printf-style formated error message and exit.
 */
#define protocol_error(format, ...) \
	error_internal(0, NULL, 0, NULL, \
		"Protocol error (" format ")", ##__VA_ARGS__)

/**
 * Shortcut for a long func name.
 */
#define j_o_o_g(obj, key) \
	json_object_object_get((obj), (key))

/**
 * Return a float from JSON object, check type,
 * protocol_error() on problems.
 */
static float load_float(struct json_object *obj){
	if(!obj){
		protocol_error("Missing numeric value.");
	}

	if(json_object_is_type(obj, json_type_string)){
		const char *str = json_object_get_string(obj);
		if(!strcmp(str, "inf")){
			return INFINITY;
		}else if(!strcmp(str, "-inf")){
			return -INFINITY;
		}else{
			// do nothing here, the invalid value
			// will be handled later.
		}
	}

	if(!json_object_is_type(obj, json_type_int) &&
		!json_object_is_type(obj, json_type_double)){
		protocol_error("Value must be a number.");
	}

	return json_object_get_double(obj);
}

/**
 * Return a int from JSON object, check type,
 * protocol_error() on problems.
 */
static int load_int(struct json_object *obj){
	if(!obj){
		protocol_error("Missing int value.");
	}

	if(!json_object_is_type(obj, json_type_int)){
		protocol_error("Value must be an integer.");
	}

	return json_object_get_int(obj);
}

/**
 * Return a const char * from a JSON object, check type.
 * The returned memory is managed by the JSON object.
 * Protocol_error() on problems.
 */
static const char *load_string(struct json_object *obj){
	if(!obj){
		protocol_error("Missing string value.");
	}

	if(!json_object_is_type(obj, json_type_string)){
		protocol_error("Value must be an string.");
	}

	return json_object_get_string(obj);
}

/**
 * Load a transformation matrix from JSON object, check type,
 * protocol_error() on problems.
 */
static void load_transformation(struct json_object *obj, struct transform *t){
	if(!obj){
		protocol_error("Missing transformation.");
	}

	if(!json_object_is_type(obj, json_type_array)){
		protocol_error("Transformation must be an array.");
	}

	int length = json_object_array_length(obj);

	if(length != 12){
		protocol_error("Transformation must have exactly 12 items.");
	}

	transform_identity(t);

	for(int i = 0; i < 4; ++i){
		for(int j = 0; j < 3; ++j){
 			t->row[i].f[j] = load_float(json_object_array_get_idx(obj, 3 * i + j));
		}
	}

}

/**
 * Load surface settings for an object.
 */
static void load_surface(struct json_object *json, struct object *obj){
	const char *surface = load_string(j_o_o_g(json, "surface"));

	if(!strcmp(surface, "lambertian")){
		lambertian_init(obj);
	}else{
		warning(0, "Unknown surface \"%s\". Ignoring.", surface);
		lambertian_init(obj);
	}
}

/**
 * Load light source settings for an object.
 */
static void load_light(struct json_object *json, struct object *obj){
	struct json_object *lightObject = j_o_o_g(json, "light");

	if(!lightObject){
		no_light_init(obj);
	}

	const char *light = load_string(lightObject);

	if(!strcmp(light, "none")){
		no_light_init(obj);
	}else if(!strcmp(light, "difuse_white")){
		difuse_white_light_init(obj);
	}else{
		warning(0, "Unknown light \"%s\". Ignoring.", light);
		no_light_init(obj);
	}
	
}

/**
 * Load an object from its JSON representation.
 * \return true if the object was succesfuly loaded.
 */
static void load_object(struct json_object *json, struct object *obj){
	struct transform t;

	load_transformation(j_o_o_g(json, "transform"), &t);

	const char *type = load_string(j_o_o_g(json, "type"));
	
	if(!strcmp(type, "cayley")){
		cayley_init(obj, &t);
	}else if(!strcmp(type, "sphere")){
		sphere_init(obj, &t);
	}else if(!strcmp(type, "plane")){
		plane_init(obj, &t);
	}else{
		warning(0, "Unknown object type \"%s\". Ignoring.", type);
	}

	load_surface(json, obj);
	load_light(json, obj);
}

/**
 * Load objects from a JSON array to an array.
 * \param array Pointer to the array.
 * It will be reallocated to hold all the objects.
 */
static struct wrapped_object *load_objects(struct json_object *json){
	if(json == NULL){
		protocol_error("Missing description of objects.");
	}

	int count = json_object_array_length(json);

	struct wrapped_object *ret = NULL;
	
	for(int i = 0; i < count; ++i){
		struct wrapped_object *tmp =
			checked_malloc(sizeof(struct wrapped_object));

		load_object(json_object_array_get_idx(json, i), &(tmp->o));

		tmp->next = ret;
		ret = tmp;
	}

	return ret;
}

/**
 * Read the camera parameters from JSON object.
 */
static void load_camera(struct json_object *camera, struct scene *s){
	if(camera == NULL){
		protocol_error("Missing camera description.");
	}

	s->raysPerPx = load_int(j_o_o_g(camera, "raysPerPx"));
	s->focalLength = load_float(j_o_o_g(camera, "focalLength"));
	s->apertureDiameter = load_float(j_o_o_g(camera, "apertureDiameter"));

	s->focus = load_float(j_o_o_g(camera, "focus"));

	s->sensorWidth = load_float(j_o_o_g(camera, "sensorWidth"));
}

/**
 * Build the scene from JSON tree.
 */
void protocol_load_scene(struct json_object *json, struct scene *s){
	scene_init(s);

	s->width = load_int(j_o_o_g(json, "width"));
	s->height = load_int(j_o_o_g(json, "height"));

	load_camera(j_o_o_g(json, "camera"), s);

	struct wrapped_object *objs = load_objects(j_o_o_g(json, "objects"));

	scene_set_objects(s, objs);
}

/**
 * Extract the message type from a message.
 */
enum protocol_msg_types protocol_load_msg_type(struct json_object *json){
	const char *type = load_string(j_o_o_g(json, "type"));

	if(!strcmp(type, "scene")){
		return MSG_TYPE_SCENE;
	}
	if(!strcmp(type, "chunk")){
		return MSG_TYPE_CHUNK;
	}
	if(!strcmp(type, "finished")){
		return MSG_TYPE_FINISHED;
	}else{
		protocol_error("Unknown message type.");
	}
}

/**
 * Load a chunk structure from a message.
 */
void protocol_load_chunk(struct json_object *json, struct renderer_chunk *chunk){
	chunk->top = load_int(j_o_o_g(json, "top"));
	chunk->height = load_int(j_o_o_g(json, "height"));
}

/**
 * Returns a JSON object containing the hello message.
 */
struct json_object *protocol_store_hello(const char *hostname){
	json_object *message = json_object_new_array();
	json_object_array_add(message, json_object_new_string("Hello"));
	json_object_array_add(message, json_object_new_string(hostname));
	
	json_object *version = json_object_new_array();
	json_object_array_add(version, json_object_new_int(CLIENT_VERSION_MAJOR));
	json_object_array_add(version, json_object_new_int(CLIENT_VERSION_MINOR));
	json_object_array_add(version, json_object_new_int(CLIENT_VERSION_REV));

	json_object_array_add(message, version);

	return message;

}

/**
 * Return a json object containing the given pixmap.
 * \param pixmap Array of colors.
 * \param length Length of the array.
 */
struct json_object *protocol_store_pixmap(const struct color *pixmap, size_t length){
	json_object *message = json_object_new_array();

	for(size_t i = 0; i < length; ++i){
		json_object *triplet = json_object_new_array();

		json_object_array_add(triplet,
			json_object_new_double(pixmap[i].r));
		json_object_array_add(triplet,
			json_object_new_double(pixmap[i].g));
		json_object_array_add(triplet,
			json_object_new_double(pixmap[i].b));

		json_object_array_add(message, triplet);
	}

	return message;
}
