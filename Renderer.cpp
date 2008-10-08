#include "Renderer.h"

Renderer::~Renderer(){
	clear_pool();
}

Intersection *Renderer::intersection_from_pool(){
	if(pool){
		Intersection *ret = pool;

		pool = pool -> next;
		return ret;
	}else
		return new Intersection();
}

void Renderer::intersection_to_pool(Intersection *i){
	i -> next = pool;
	pool = i;
}

void Renderer::intersections_to_pool(Intersection *first, Intersection *last){
	last -> next = pool;
	pool = first;
}

void Renderer::clear_pool(){
	while(pool){
		Intersection *tmp = pool -> next;
		delete pool;
		pool = tmp;
	}
}

void Renderer::render(Scene *s, Pixmap *p, unsigned int fromRow, unsigned int toRow){
	Colour *pixel = p -> getRow(fromRow);
	for(unsigned int y = fromRow; y < rowTo; ++y){
		for(unsigned int x = 0; x < p -> get_width(); ++x){
//			Scene -> 
			++ pixel;
		}
	}
}
