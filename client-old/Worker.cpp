#include "Worker.h"

Worker::~Worker(){
	clear_pool();
}

Intersection *Worker::intersection_from_pool(){
	if(pool){
		Intersection *ret = pool;

		pool = pool -> next;
		return ret;
	}else
		return new Intersection();
}

void Worker::intersection_to_pool(Intersection *i){
	i -> next = pool;
	pool = i;
}

void Worker::intersections_to_pool(Intersection *first, Intersection *last){
	last -> next = pool;
	pool = first;
}

void Worker::clear_pool(){
	while(pool){
		Intersection *tmp = pool -> next;
		delete pool;
		pool = tmp;
	}
}

void Worker::render(Scene *s, Pixmap *p, unsigned int fromRow, unsigned int toRow){
	Colour *pixel = p -> getRow(fromRow);
	for(unsigned int y = fromRow; y < rowTo; ++y){
		for(unsigned int x = 0; x < p -> get_width(); ++x){
//			Scene -> 
			++ pixel;
		}
	}
}
