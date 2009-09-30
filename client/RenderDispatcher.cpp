#include "RenderDispatcher.h"

using namespace std;

RenderDispatcher() : granularity(0), progress(0), threads(0), scene(NULL), Pixmap(NULL) {}

/// Start rendering using all available threads.
void RenderDispatcher::go(){
	Chunk tmp;
	tmp.y0 = 0;
	for(i = get_available_threads() * granularity; i >= 1; --i){
		tmp.y1 = (pixmap->get_height() - tmp.y0) / i;
		waitingChunks.push_back(tmp);
		tmp.y0 = tmp.y1;
	}

	progress = 0;

	vector<RenderServerConnection *>::iterator it;

	for(it = connections.begin(); it != connections.end(); ++it){
		(*it)->set_scene(scene);
		for(int j = 0; j < (*it)->get_available_threads(); ++j)
			set_job(it);
	}
}

/// Set a new job for a single thread of the given server.
void RenderDispatcher::set_job(RenderServerConnection *server){
	Chunk tmp = waitingChunks.pop_front();
	server->render(pixmap->get_width(), tmp.y0, tmp.y1);
}
