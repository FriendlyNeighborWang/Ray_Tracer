#include "rtweekend.h"
#include "Displayer.h"
#include "Hittable.h"
#include "Hittable_list.h"
#include "Sphere.h"
#include "Camera.h"
#include <fstream>

int main() {
	hittable_list world;
	world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
	world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

	camera cam;
	cam.aspect_ratio = 16.0 / 9.0;
	cam.image_width = 800;
	cam.samples_per_pixel = 100;

	//Êä³öÎÄ¼ş
	std::ofstream out("output.ppm");

	cam.render(world, out);

	display("output.ppm");
}

