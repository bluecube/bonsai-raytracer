#ifndef PIXMAP_H
#define PIXMAP_H

#include "Colour.h"

class Pixmap{
public:
	enum{
		MAPPING_CLIP,
	};

	Pixmap(unsignd int width, usigned int height);
	~Pixmap():

	size_t get_width(){
		return w;
	}

	size_t get_height(){
		return h;
	}

	inline Coulour *get_row(unsigned int row){
		return pixmap + w * row;
	}

	inline Colour &operator()(unsigned int x, unsigned int y){
		return pixmap[x + y * w];
	}

	void save(const char *file, int mapping = MAPPING_CLIP);
private
	void clip_mapping(DATA32 *data);

	DATA8 clip_value(double value);

	Colour *pixmap;
	size_t w, h;
};

#endif
