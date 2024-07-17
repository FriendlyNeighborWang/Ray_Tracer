#pragma once
#include "rtweekend.h"
#include "Hittable.h" // ��Դ���벻ͬ

class material {
public:
	virtual ~material() = default;

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

// ���������
class lambertian :public material {
public:
	lambertian(const color& albedo):albedo(albedo){}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		//��������ߵ��߼�ΪLambertian Reflection��ʹ������õ��Ĺ��߸��ӽӽ�����
		auto scatter_direction = rec.normal + random_unit_vector();

		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;

		scattered = ray(rec.p, scatter_direction);
		attenuation = albedo;
		return true;
	}

private:
	color albedo;
};

class metal : public material {
public:
	//ͨ���Է�����߽���һ����Χ�����ƫ�ƣ�ʹ�ý����ı��濴������΢ģ��һЩ������ĥɰ�ʸ�
	metal(const color& albedo, double fuzz = 0.0):albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1){}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
		scattered = ray(rec.p, reflected);
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

private:
	color albedo;
	double fuzz;
};

//͸������
class dielectric :public material {
public:
	dielectric(double refraction_index):refraction_index(refraction_index){}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		attenuation = color(1.0, 1.0, 1.0);
		double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

		vec3 unit_direction = unit_vector(r_in.direction());
		double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		// �������п��ܷ���ȫ������������Ҫ�Ƚ����Ƿ�ᷢ��������ж�
		bool cannot_recfract = ri * sin_theta > 1.0;
		vec3 direction;

		if (cannot_recfract||reflectance(cos_theta,ri)>random_double())
			direction = reflect(unit_direction, rec.normal);
		else
			direction = refract(unit_direction, rec.normal, ri);

		scattered = ray(rec.p, direction);

		return true;
	}

private:
	double refraction_index;

	// ����ͬʱҲ�в��ַ��������
	static double reflectance(double cosine, double refraction_index) {
		auto r0 = (1 - refraction_index) / (1 + refraction_index);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};