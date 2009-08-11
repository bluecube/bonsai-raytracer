#ifndef PIXMAP_H
#define PIXMAP_H

#include <cstdlib>

#include "Colour.h"

/// Class representing a HDR pixmap.
class Pixmap{
public:
#ifdef WITH_PIXMAP_SAVE
	enum{
		MAPPING_CLIP,
		MAPPING_LINEAR,
	};
#endif

	Pixmap(unsignd int width, usigned int height);
	~Pixmap():

	/// Return the width of the pixmap.
	size_t get_width(){
		return w;
	}

	/// Return the Height of the pixmap.
	size_t get_height(){
		return h;
	}
	
	/// Return a pointer to a row of pixmap.
	/// Doesn't perform any checking.
	Coulour *get_row(size_t row){
		return pixmap + w * row;
	}
	
	/// Alias to get_row.
	Colour *operator()(size_t row){
		return get_row(row);
	}
	
	/// Return a reference to a single pixel.
	/// Doesn't perform any checking.
	Colour &operator()(size_t x, size_t y){
		return pixmap[x + y * w];
	}

	/// Copy rows from source to the current pixmap.
	/// No checks are performed.
	/// \pre this->get_width() == source->get_width()
	/// \pre targetRow + count <= this->get_height()
	/// \pre sourceRow + count <= source->get_height()
	/// \param source Source pixmap.
	/// \param sourceRow First row in source that will be copied.
	/// \param targetRow First row that will be overwritten.
	/// \param count Number of rows copied.
	void copy_rows(const Pixmap *source, size_t sourceRow, size_t targetRow, size_t count){
		memcpy(get_row(targetRow), source->get_row(sourceRow), count * w * sizeof(Colour));
	}

	/// Copy all rows from source to the current pixmap.
	/// No checks are performed.
	/// \pre this->get_width() == source->get_width()
	/// \pre targetRow + source->get_height() <= this->get_height()
	/// \param source Source pixmap.
	/// \param targetRow First row that will be overwritten.
	void copy_rows(const Pixmap *source, size_t targetRow){
		copy_rows(source, 0, targetRow, source->get_height());
	}

#ifdef WITH_PIXMAP_SAVE
	void save(const char *file, int mapping = MAPPING_CLIP);
	void save_hdr(consr char *file);
private:
	void clip_mapping(DATA32 *data);
	void linear_mapping(DATA32 *data);

	void get_extremes(double *min, double *max);
	
	int clip_value(double value);
#else
private:
#endif

	Colour *pixmap;
	size_t w, h;
};

#endif
