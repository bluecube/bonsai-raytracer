#include "Pixmap.h"

Pixmap::Pixmap(size_t width, size_t height) : w(width), h(height) {
	pixmap = new Coulour[w * h];
}

Pixmap::~Pixmap(){
	delete[] pixmap;
}

/// Save the pixmap as a image using imlib2.
/// \param file Filename of the image. The extension determines type.
/// \param mapping Method for mapping high dynamic range colors into the
/// image. Now only MAPPING_CLIP works.
void Pixmap::save(const char *file, int mapping){
	Imlib_Image buffer = imlib_create_image(w, h);
	imlib_context_set_image(buffer);
	DATA32 *data = imlib_image_get_data();

	//switch(mapping){
	//	case MAPPING_CLIP:
			clip_mapping(data); break;
	//}

	imlib_image_put_back_data(data);

	const char *format = strrchr(file, '.') + 1;
	imlib_image_set_format(format);
	imlib_save_image(file);
	imlib_free_image();
}

void Pixmap::clip_mapping(DATA32 *data){
	Colour *orig = pixmap;
	for(size_t y = 0; y < h; ++y){
		for(size_t x = 0; x < w; ++x){
			*data = (clip_value(orig->r) << 16) |
				(clip_value(orig->g) << 8) |
				(clip_value(orig->b);

			++orig;
			++data;
		}
	}
}

DATA8 clip_value(double value){
	if(value > 1)
		return 255;
	if(value < 0)
		return 0;
	
	return static_cast<DATA8>(value * 255);
}
