#pragma once
#include "Ray.h"

class hit_record {
public:
	point3 p;
	vec3 normal;
	double t;
	double front_face;

	void set_face_normal(const ray& r, const vec3& outward_nomral){
		front_face = dot(r.direction(), outward_nomral) < 0;
		normal = front_face ? outward_nomral : -outward_nomral;
	}
};

class hittable {
public:
	virtual ~hittable() = default;
	//光的时间是无限的，所以这里的t_min, t_max是一个区间，只有碰撞发生在这个时间区间之内才会被记录
	virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};