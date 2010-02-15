#ifndef RENDERDISPATCHER_H
#define RENDERDISPATCHER_H

#include <stdlib.h>

#include <vector>

#include "client/RenderServerConnection.h"
#include "common/Scene.h"
#include "common/Pixmap.h"

class RenderDispatcher{
public:
	RenderDispatcher();
	
	/// Add a connection to be used for rendering.
	void add_connection(RenderServerConnection *con){
		connections.push_back(con);
		con->set_dispatcher(this);
	}
	
	/// Set the number of chunks per available thread.
	/// The smaller the granularity is, the smaller is the overhead
	/// of client-server communication.
	/// Bigger granularity allows ballancing
	/// between stronger and weaker servers and between simple/complicated
	/// parts of the scene.
	/// \note No chunk can be smaller than 1 row.
	void set_granularity(unsigned granularity_){
		granularity = granularity_;
	}
	
	/// Get the number of threads available in connected servers.
	int get_available_threads(){
		return threads;
	}

	/// Return the progress as a number between 0 (not completed)
	/// and 1 (complete).
	double get_progress(){
		return (chunks - get_waiting_chunks() - get_working_chunks()) / (double)chunks;
	}
	
	/// Return the number of chunks waiting for processing.
	unsigned get_waiting_chunks(){
		return waitingChunks.size();
	}
	
	/// Return the number of chunks being processed.
	unsigned get_working_chunks(){
		return workingChunks.size();
	}

	/// Return the number of chunks.
	unsigned get_chunks(){
		return chunks;
	}
	
	/// Are we finished with rendering?
	bool is_finished(){
		return !waitingChunks.size() && !workingChunks.size();
	}

	void go();
	
	/// Set the scene that will be rendered with the next call to go().
	void set_scene(Scene *sc){
		scene = sc;
	}

	/// Set the pixmap that will keep the result.
	void set_pixmap(Pixmap *p){
		pixmap = p;
	}

	void wait_for_update();
private:
	void set_job(RenderServerConnection *server, int thread);

	/// Structure representing a chunk that is rendered by a single thread.
	/// A chunk are lines within <y0, y1).
	struct Chunk{
		unsigned y0, y1;
	};
	
	unsigned granularity;

	std::vector<Chunk> waitingChunks;
	std::vector<Chunk> workingChunks;
	unsigned chunks;

	std::vector<RenderServerConnection *> connections;
	
	int threads;

	Scene *scene;
	Pixmap *pixmap;
};

#endif
