#ifndef PIXMAP_H
#define PIXMAP_H

class Pixmap{
public:
	Pixmap(unsignd int width, usigned int height);
	~Pixmap():

	inline Coulour *get_row(unsigned int row){
		return pixmap + w * row;
	}

	inline Colour &operator()(unsigned int x, unsigned int y){
		return pixmap[x + y * w];
	}

	void compress_HDR();
	void write_png(char *fn);
private:
	Colour *pixmap;
	unsigned int w, h;
};

#endif
