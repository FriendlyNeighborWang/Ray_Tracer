#pragma once
#include "rtweekend.h"
#include "Hittable.h"
#include "Material.h"
#include <fstream>

class camera {
public:
	double aspect_ratio = 1.0;
	int image_width = 100;
	int samples_per_pixel = 10;
	int max_depth = 10;

	double vfov = 90;
	point3 lookfrom = point3(0, 0, 0);  //相机所在点
	point3 lookat = point3(0, 0, -1);  //相机观察方向(也是屏幕中心点)
	vec3 vup = vec3(0, 1, 0);  //全局“上”方向向量


	void render(const hittable& world, std::ofstream& out) {
		initialize();
		
		//render
		out << "P3\n" << image_width << " " << image_height << "\n255\n";
		for (int j = 0; j < image_height; ++j) {
			std::clog << "\rScanlines remianing:" << (image_height - j) << " " << std::flush;
			for (int i = 0; i < image_width; i++) {
				color pixel_color(0, 0, 0);
				for (int sample = 0; sample < samples_per_pixel; sample++) {
					ray r = get_ray(i, j);
					pixel_color += ray_color(r, max_depth, world);
				}
				write_color(out, pixel_samples_scale * pixel_color);
			}
		}
		std::clog << "\rDone.               \n";

		out.close();
	}
private:
	int image_height;
	double pixel_samples_scale;
	point3 center;
	point3 pixel00_loc;
	vec3 pixel_delta_u;
	vec3 pixel_delta_v;
	vec3 u, v, w;

	void initialize() {
		//计算图像长宽
		image_height = int(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;
		
		pixel_samples_scale = 1.0 / samples_per_pixel;

		center = lookfrom;  //相机中心

		//相机
		auto focal_length = (lookfrom - lookat).length();  //焦距
		auto theta = degrees_to_radians(vfov);
		auto h = tan(theta / 2);
		auto viewport_height = 2 * h * focal_length;  //视口高
		auto viewport_width = viewport_height * (double(image_width) / image_height);
		

		//相对相机自身观察向量
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		//视口
		auto viewport_u = viewport_width * u;
		auto viewport_v = viewport_height * -v;

		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		auto viewport_upper_left = center - (focal_length * w) - viewport_u / 2 - viewport_v / 2;
		pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
		
	}

	ray get_ray(int i, int j) const {
		auto offset = sample_square();
		auto pixel_sample = pixel00_loc
			+ ((i + offset.x()) * pixel_delta_u)
			+ ((j + offset.y()) * pixel_delta_v);

		auto ray_origin = center;
		auto ray_direction = pixel_sample - ray_origin;

		return ray(ray_origin, ray_direction);
	}

	vec3 sample_square() const {
		return vec3(random_double() - 0.5, random_double() - 0.5, 0);
	}

	color ray_color(const ray& r, int depth, const hittable& world) const {
		//如果超过最大深度则默认为黑色
		if (depth <= 0)
			return color(0, 0, 0);

		hit_record rec;
		//因为浮点数的精度影响，交点很有可能不够精确，这样的点在球面下（球内）时有可能导致
		//光线在内部与球面相交，时间很短，做法是忽略那些发生时间过短的相交，能够有效解决shadow acne问题
		if (world.hit(r, interval(0.001, infinity), rec)) {
			ray scattered;
			color attenuation;
			if (rec.mat->scatter(r, rec, attenuation, scattered))
				return attenuation * ray_color(scattered, depth - 1, world);
			return color(0, 0, 0);
		}
		vec3 unit_direction = unit_vector(r.direction());
		auto a = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
	}
};