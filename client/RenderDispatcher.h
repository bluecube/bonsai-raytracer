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

	void add_connection(RenderServerConnection *con){
		connections.push_back(con);
		threads += con->get_available_threads();
	}
	
	/// Set the number of chunks per available thread.
	void set_granularity(unsigned granularity_){
		granularity = granularity_;
	}

	int get_available_threads(){
		return threads;
	}

	/// Return the progress as a number between 0 (not completed)
	/// and 1 (complete).
	double get_progress(){
		return progress / pixmap->get_height();
	}

	unsigned get_waiting_chunks(){
		return waiting.size();
	}

	bool is_finished(){
		return currentChunkY < 0 && !working;
	}

	void go();

	void set_scene(Scene *sc){
		scene = sc;
	}

	void set_pixmap(Pixmap *p){
		pixmap = p;
	}

	void wait_for_update();
private:
	struct Chunk{
		unsigned y0, y1;
	};

	unsigned granularity;

	std::vector<Chunk> waitingChunks;
	std::vector<Chunk> workingChunks;
	

	std::vector<RenderServerConnection *> connections;

	size_t progress;
	
	int threads;

	Scene *scene;
	Pixmap *pixmap;
};

#endif
