#ifndef RENDERSETTINGS_H
#define RENDERSETTINGS_H

/// Class containing the global rendering settings for the raytracer.
class RenderSettings{
public:
	/// Return \(f/#\) of the camera.
	double get_f_number(){
		return focal_length / (2 * aperture_radius);
	}

	/// Returns the radius of the camera's aperture.
	double get_aperture_radius(){
		return aperture_radius;
	}
	
	/// Returns the focal length in milimeters.
	double get_focal_length(){
		return focal_length;
	}

	/// Returns number of samples rendered per every pixel of the resulting image.
	size_t get_sample_count(){
		return sample_count;
	}

	/// Returns how many pixels will be on average in every square millimeter of aperture.
	/// There will always be at least one sample, even if the aperture has zero radius
	/// (ideal pinhole camera).
	size_t get_sample_count_per_aperture_sq_mm(){
		return (sample_cout - 1) /
			(aperture_radius * aperture_radius * M_PI);
	}
}

#endif
