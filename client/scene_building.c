#include "scene_building.h"

#include <math.h>
#include <string.h>

#include "sphere.h"
#include "transform.h"
#include "util.h"

/**
 * Return a float from JSON object, check type.
 */
static float float_from_json(struct json_object *obj){
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
 * Return a int from JSON object, check type.
 */
static int int_from_json(struct json_object *obj){
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
 */
static const char *string_from_json(struct json_object *obj){
	if(!obj){
		protocol_error("Missing string value.");
	}

	if(!json_object_is_type(obj, json_type_string)){
		protocol_error("Value must be an string.");
	}

	return json_object_get_string(obj);
}

/**
 * Load a transformation matrix from JSON object, check type.
 */
static void transformation_from_json(struct json_object *obj, struct transform *t){
	if(!obj){
		protocol_error("Missing transformation.");
	}

	if(!json_object_is_type(obj, json_type_array)){
		protocol_error("Transformation must be an array.");
	}

	int length = json_object_array_length(obj);

	if(length != TRANSFORM_SIZE){
		protocol_error("Transformation must have exactly %d items.", TRANSFORM_SIZE);
	}

	for(int i = 0; i < TRANSFORM_SIZE; ++i){
		t->p[i] = float_from_json(json_object_array_get_idx(obj, i));
	}
}

/**
 * Read the camera parameters from JSON object.
 */
static void load_camera(struct json_object *camera, struct scene *s){
	if(camera == NULL){
		protocol_error("Missing camera description.");
	}

	s->raysPerPx = int_from_json(json_object_object_get(camera, "raysPerPx"));
	s->focalLength = float_from_json(json_object_object_get(camera, "focalLength"));
	s->apertureDiameter = float_from_json(json_object_object_get(camera, "apertureDiameter"));
}

/**
 * Return a new object loaded from its JSON representation.
 */
static struct object *load_single_object(struct json_object *json){
	struct object *ret = NULL;

	struct transform t;

	transformation_from_json(json_object_object_get(json, "transform"), &t);

	const char *type = string_from_json(json_object_object_get(json, "type"));
	if(!strcmp(type, "sphere")){
		ret = sphere_new(&t);
	}else{
		warning(0, "Unknown object type \"%s\". Ignoring.", type);
		return NULL;
	}

	return ret;
}

/**
 * Load objects from a JSON array to a simple linked list.
 */
static void load_objects(struct json_object *json, struct object **objects){
	if(json == NULL){
		protocol_error("Missing description of objects.");
	}

	int count = json_object_array_length(json);
	
	for(int i = 0; i < count; ++i){
		struct object *obj = load_single_object(json_object_array_get_idx(json, i));
		if(obj == NULL){
			continue;
		}
		obj->next = *objects;
		*objects = obj;
	}
}

/**
 * Build the scene from JSON tree.
 */
struct scene *scene_build(struct json_object *json){
	struct scene *ret = checked_malloc(sizeof(*ret));

	ret->width = int_from_json(json_object_object_get(json, "width"));
	ret->height = int_from_json(json_object_object_get(json, "height"));

	load_camera(json_object_object_get(json, "camera"), ret);
	load_objects(json_object_object_get(json, "objects"), &(ret->objs));

	return ret;
}
