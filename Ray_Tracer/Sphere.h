#pragma once
#include "Hittable.h"
#include "rtweekend.h"

class sphere :public hittable {
public:
	sphere(const point3& c, const double& r) :center(c), radius(fmax(0, r)){}
	sphere(point3&& c, double&& r):center(c),radius(fmax(0,r)){}

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
		vec3 oc = center - r.origin();
		auto a = r.direction().length_squared();
		//b = -2h;
		auto h = dot(r.direction(), oc);
		auto c = oc.length_squared() - radius * radius;
		auto discriminant = h * h - a * c;

		if (discriminant < 0)
			return false;
		auto sqrtd = sqrt(discriminant);

		auto root = (h - sqrtd) / a;
		if (!ray_t.surrounds(root)) {
			root = (h + sqrtd) / a;
			if (!ray_t.surrounds(root))
				return false;
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		vec3 outward_normal = (rec.p - center) / radius;
		rec.set_face_normal(r, outward_normal);

		return true;
	}
private:
	point3 center;
	double radius;
};
