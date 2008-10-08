#include "Pixmap.h"

Pixmap::Pixmap(unsignd int width, usigned int height) : w(width), h(height) {
	pixmap = new Coulour[w * h];
}

Pixmap::~Pixmap(){
	delete[] pixmap;
}
