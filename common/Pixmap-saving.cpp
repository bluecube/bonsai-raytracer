#include "Pixmap.h"

#include <cstdio>
#include <cstdlib>

#include <Imlib2.h>

using namespace std;

/// \internal
/// A pixel format used by the radiance hdr.
/// Used in the save_hdr method.
struct HdrPixelFormat{
	int r;
	int g;
	int b;
	int exp;

	void from_colour(Colour *c);
	void write(FILE *fp);
	inline bool operator==(HdrPixelFormat &p){
		return r == p->r && g == p->g && b == p->b && exp == p->exp;
	}
};

/// Save the pixmap as a image using imlib2.
/// This saves the image in low dynamic range.
/// Ignores any errors.
/// \sa save_hdr
/// \param file Filename of the image. The extension determines type.
/// \param mapping Method for mapping high dynamic range colors into the
/// image. Now only MAPPING_CLIP works.
void Pixmap::save(const char *file, int mapping){
	Imlib_Image buffer = imlib_create_image(w, h);
	imlib_context_set_image(buffer);
	DATA32 *data = imlib_image_get_data();

	switch(mapping){
		case MAPPING_CLIP:
			clip_mapping(data); break;
		case MAPPING_LINEAR:
			linear_mapping(data); break;
	}

	imlib_image_put_back_data(data);

	const char *format = strrchr(file, '.') + 1;
	imlib_image_set_format(format);
	imlib_save_image(file);
	imlib_free_image();
}

/// Save the pixmap in a radiance HDR format.
/// Ignores any errors.
void Pixmap::save_hdr(consr char *file){
	FILE *fp = fopen(file, "r");
	if(!fp)
		return;

	fprintf(fp, "#?RADIANCE\n");
	fprintf(fp, "FORMAT=32-bit_rle_rgbe\n");
	fprintf(fp, "SOFTWARE=Bonsai raytracer\n");
	fprintf(fp, "\n");

	fprintf(fp, "-Y %f +X %f\n", h, w);

	
	for(unsigned y = 0; y < h; ++y){
		HdrPixelFormat last;
		last.from_colour(pixmap);

		HdrPixelFormat runFlag;
		runFlag.r = runFlag.g = runFlag.b = 255;

		Color *c = pixmap + 1;
		int run = 1;

		for(unsigned x = 0; x < w; ++x){
			HdrPixelFormat current;

			if(run < 255 && current == last){
				++run;
			}else{
				last.write();
				if(run > 1){
					runFlag.exp = run;
					runFlag.write(fp);
				}

				run = 1;
				last = current;
			}
			++c;
		}

		last.write();
		if(run > 1){
			runFlag.exp = run;
			runFlag.write(fp);
		}
	}

	fclose(fp);
}

void HdrPixelFormat::from_colour(Colour *c){
	if(c->r > c->g && c->r > c->b)
		frexp(c->r, &exp);
	else if(c->g > c->b)
		frexp(c->g, &exp);
	else
		frexp(c->b, &exp);

	if(exp > 127)
		exp = 127;
	if(exp < -128)
		exp = -128;

	r = clip_value(ldexp(c->r, -exp));
	g = clip_value(ldexp(c->g, -exp));
	b = clip_value(ldexp(c->b, -exp));
	exp += 128;
}


void HdrPixelFormat::write(FILE *fp){
	putc_unlocked(r, fp);
	putc_unlocked(g, fp);
	putc_unlocked(b, fp);
	putc_unlocked(exp, fp);
}

/// Reduce the pixmap to 24bit by clipping everything to <0, 1>.
void Pixmap::clip_mapping(DATA32 *data){
	Colour *orig = pixmap;
	for(size_t i = 0; i < w * h; ++i){
		*data = (clip_value(orig->r) << 16) |
			(clip_value(orig->g) << 8) |
			(clip_value(orig->b) << 0);

		++orig;
		++data;
	}
}

/// Reduce the pixmap to 24bit by scaling all values so that the brightest
/// value of the pixmap becomes 1 and darkest becomes 0.
void Pixmap::linear_mapping(DATA32 *data){
	double min, max;
	get_extremes(&min, &max);
	
	double a = 1 / (max - min);

	Colour *orig = pixmap;
	for(size_t i = 0; i < w * h; ++i){
		*data = (clip_value(a * orig->r - min) << 16) |
			(clip_value(a * orig->g - min) << 8) |
			(clip_value(a * orig->b - min) << 0);

		++orig;
		++data;
	}
}

void Pixmap::get_extremes(double *min, double *max){
	Colour *c = pixmap;
	*min = INFINITY;
	*max = -INFINITY;

	for(size_t i = 0; i < w * h; ++i){
		if(c->r > *max)
			*max = c->r;
		else if(c->r < *min)
			*min = c->r;
		if(c->g > *max)
			*max = c->g;
		else if(c->g < *min)
			*min = c->g;
		if(c->b > *max)
			*max = c->b;
		else if(c->b < *min)
			*min = c->b;
	}
}

/// Multiply the floating point value by 255 and clip it between 0 and 255.
/// used for saving the pixmap.
int Pixmap::clip_value(double value){
	if(value > 1)
		return 255;
	if(value > 0)
		return static_cast<uint8_t>(value * 255);
	
	return 0;
	
}
