#include "Pixmap.h"

#include <cstdio>
#include <cstdlib>

using namespace std;

Pixmap::Pixmap(size_t width, size_t height) : w(width), h(height) {
	pixmap = new Colour[w * h];
}

Pixmap::~Pixmap(){
	delete[] pixmap;
}
