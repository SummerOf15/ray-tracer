#pragma once
#ifndef HITABLEH
#define HITABLEH

#include "ray.h"
#include "Material.h"
#include "Overlap.h"

struct hit_record
{
	double t;//distance
	vec3 p;//hit position
	vec3 normal;// normalized vector in hit point
	material* mat_ptr;
};

class Hitable
{
public:
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
	virtual bool bounding_box(float t0, float t1, Overlap& box) const = 0;
};
class flip_normals : public Hitable {
public:
	flip_normals(Hitable* p) : ptr(p) {}
	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
		if (ptr->hit(r, t_min, t_max, rec)) {
			rec.normal = -rec.normal;
			return true;
		}
		else
			return false;
	}
	virtual bool bounding_box(float t0, float t1, Overlap& box) const {
		return ptr->bounding_box(t0, t1, box);
	}
	Hitable* ptr;
};
#endif