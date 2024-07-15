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
	//���ʱ�������޵ģ����������t_min, t_max��һ�����䣬ֻ����ײ���������ʱ������֮�ڲŻᱻ��¼
	virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};