#ifndef RENDERDISPATCHER_H
#define RENDERDISPATCHER_H

#include "RenderServerConnection.h"

#include "Scene.h"
#include "Pixmap.h"

#include <vector>
#include <pthread.h>

class RenderDispatcher{
public:
	RenderDispatcher() : threads(0), working(0) {}

	void add_connection(RenderServerConnection *con){
		connections.push_back(con);
		threads += con -> get_available_threads();
	}

	int get_available_threads(){
		return threads;
	}

	void set_chunks(unsigned X, unsigned Y){
		chunksX = X;
		chunksY = Y;
	}

	double get_progress(){
		return progress;
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
	pthread_cond_t updated;

	bool one_chunk(RenderServerConnection *con);

	std::vector<RenderServerConnection *> connections;

	unsigned working;

	/// Server connection calls this when a chunk is finished.
	void ready(RenderServerConnection *con);

	unsigned chunksX, chunksY;
	unsigned chunkWidth, chunkHeight;

	unsigned currentChunkX, currentChunkY;

	double progress;
	
	unsigned threads;

	Scene *scene;
	Pixmap *pixmap;

	friend class RenderServerConnection;
};

#endif
