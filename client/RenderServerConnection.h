#ifndef RENDERSERVERCONNECTION_H
#define RENDERSERVERCONNECTION_H

#include <stdio.h>

#include "common/Scene.h"

class RenderDispatcher;

/// A class representing a connection to the render server.
class RenderServerConnection{
public:
	/// \arg addr address of the server in format host[:port]
	RenderServerConnection(char *addr){
		printf("server %s\n", addr);
	}

	/// Returns how many threads does the server make available.
	unsigned get_available_threads(){
		return 1;
	}

	void set_dispatcher(RenderDispatcher *d){
		dispatcher = d;
	}

	void set_scene(Scene *s);

	void render(unsigned w, unsigned y0, unsigned y1);
private:
	RenderDispatcher *dispatcher;
};

#endif
