#ifndef RENDERSERVERCONNECTION_H
#define RENDERSERVERCONNECTION_H

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
private:
	RenderDispatcher *dispatcher;
};

#endif
