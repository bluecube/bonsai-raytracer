#include "Pixmap.h"

Pixmap::Pixmap(size_t width, size_t height) : w(width), h(height) {
	pixmap = new Coulour[w * h];
}

Pixmap::~Pixmap(){
	delete[] pixmap;
}
