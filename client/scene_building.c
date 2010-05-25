#include "scene_buliding.h"

#include "util.h"

/**
 * Return a float with a given key from JSON object.
 */
static float float_from_json_obj(struct json_object *obj, const char *key){
	obj = json_object_object_get(obj, key);

	if(!obj){
		error(0, "Protocol error (Missing float value \"%s\").", key);
	}

	return json_object_get_double(obj);
}

/**
 * Return a int with a given key from JSON object.
 */
static int int_from_json_obj(struct json_object *obj, const char *key){
	obj = json_object_object_get(obj, key);

	if(!obj){
		error(0, "Protocol error (Missing int value \"%s\").", key);
	}

	return json_object_get_int(obj);
}

static void transformation_from_json_obj(struct json_object *obj,
	const char *key, struct transform *t){

	obj = json_object_object_get(obj, key);

	if(!obj){
		error(0, "Protocol error (Missing transformation matrix \"%s\").", key);
	}
}

/**
 * Read the camera parameters from JSON object.
 */
static void load_camera(struct scene *s, struct json_object *camera){
	s->sampleCount = int_from_json_obj(camera, "sampleCount");
	s->focalLength = float_from_json_obj(camera, "focalLength");
	s->apertureDiameter = float_from_json_obj(camera, "apertureDiameter");
}

/**
 * Load a single object from its JSON representation.
 */
static struct object *load_single_object(struct json_object json){
	struct object *ret = NULL;

	struct transform 

	struct json_object *typeObj = json_object_object_get(json, "type");
	if(typeObj == NULL){
		error(0, "Protocol error (object type not specified).");
	}
	const char *type = json_object_get_string(typeObj);

	if(!strcmp(type, "sphere")){
		ret = sphere_new();
	}else{
		warning(0, "Unknown object type \"%s\". Ignoring.", type);
		return NULL;
	}
}

/**
 * Load objects from a JSON array to a simple linked list.
 */
static void load_objects(struct object **objects, struct json_object *json){
	int count = json_object_array_length(json);
	
	for(int i = 0; i < count; ++i){
		struct object obj = load_single_object(json_object_array_get_idx(json, i));
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
	struct scene_t ret = checked_malloc(sizeof(*ret));

	s->width = int_from_json_obj(json, "width");
	s->height = int_from_json_obj(json, "height");

	struct json_object *camera = json_object_object_get(json, "camera");
	if(!camera){
		error(0, "Protocol error (Missing camera description).");
	}
	load_camera(ret, camera);
	
	struct object *objects = NULL;
	struct json_object *objectsJson = json_object_object_get(json, "objects");
	if(!objectsJson){
		error(0, "Protocol error (Missing objects description).");
	}
	load_objects(&objects, objectsJson);
}
