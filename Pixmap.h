#ifndef PIXMAP_H
#define PIXMAP_H

class Pixmap{
public:
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

	void compress_hdr();
private:
	Colour *pixmap;
	size_t w, h;
};

#endif
