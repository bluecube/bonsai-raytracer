#include "RenderDispatcher.h"

using namespace std;

/// Start rendering using all available threads.
void RenderDispatcher::go(){
	working = 0;
	waiting.erase();
	progress = 0;

	chunkWidth = 1 + pixmap->get_width / chunksX;
	chunkHeight = 1 + pixmap->get_height / chunksY;

	currentChunkX = chunksX - 1;
	currentChunkY = chunksY - 1;

	vector<RenderServerConnection *>::iterator i;

	for(i = connections.begin(); i != connections.end(); ++i){
		(*i) -> set_scene(scene);
		(*i) -> set_pixmap(pixmap);
		for(unsigned j = 0; j < (*i) -> get_available_threads(); ++j)
			one_chunk(*i);
	}
}

/// start rendering one chunk in one thread of a given server.
void RenderDispatcher::one_chunk(RenderServerConnection *con){
	if(currentChunkY < 0)
		return;

	unsigned x1 = currentChunkX * chunkWidth;
	unsigned y2 = currentChunkY * chunkHeight;
	unsigned x2 = x1 + chunkWidth;
	unsigned y2 = y1 + chunkHeight;

	if(x2 > pixmap->get_width())
		x2 = pixmap->get_width();
	
	if(y2 > pixmap->get_height())
		y2 = pixmap->get_height();

	con -> render(x1, y1, x2, y2);

	if(currentChunkX){
		--currentChunkX;
	}else{
		currentChunkX = chunksX - 1;
		--currentChunkY;
	}
}

void RenderDispatcher::ready(RenderServerConnection * con, unsigned x1, unsigned x2, unsigned y1, unsigned y2){
	progress += (x2 - x1) * (y2 - y1) / (pixmap->get_width() * pixmap->get_height());

	
}
