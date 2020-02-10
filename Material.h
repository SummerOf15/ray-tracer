#pragma once
#include "ray.h"

struct hit_record;
class material
{
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

vec3 random_in_unit_sphere()
{
	vec3 p;
	do
	{
		p = vec3(2.0) * vec3(double(rand()) / RAND_MAX, double(rand()) / RAND_MAX, double(rand()) / RAND_MAX) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	return p;
}

