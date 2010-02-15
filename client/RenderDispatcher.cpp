#include "RenderDispatcher.h"

using namespace std;

RenderDispatcher::RenderDispatcher() : granularity(0), threads(0), scene(NULL), pixmap(NULL) {}

/// Split the pixmap into chunks and start rendering
/// using all available threads.
void RenderDispatcher::go(){
	vector<RenderServerConnection *>::iterator it;
	Chunk tmp;

	threads = 0;
	
	// check servers
	for(it = connections.begin(); it != connections.end(); ++it){
		threads += (*it)->get_available_threads();
		(*it)->set_scene(scene);
	}

	// prepare the chunks
	tmp.y0 = 0;
	chunks = threads * granularity;
	if(chunks > pixmap->get_height())
		chunks = pixmap->get_height();
	
	for(unsigned i = chunks; i >= 1; --i){
		tmp.y1 = (pixmap->get_height() - tmp.y0) / i;
		waitingChunks.push_back(tmp);
		tmp.y0 = tmp.y1;
	}
	
	// start working
	for(it = connections.begin(); it != connections.end(); ++it){
		for(int i = 0; i < (*it)->get_available_threads(); ++i)
			set_job(*it, i);
	}
}

/// Set a new job for a thread of a server.
/// @arg server Connection to the server.
/// @arg thread Thread number on the server.
void RenderDispatcher::set_job(RenderServerConnection *server, int thread){
	Chunk tmp = waitingChunks.back();
	waitingChunks.pop_back();
	workingChunks.push_back(tmp);
	server->render(pixmap->get_width(), tmp.y0, tmp.y1);
}
