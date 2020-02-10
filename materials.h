#pragma once
#include "Material.h"
#include "hitable.h"
#include "Texture.h"

class lambertian :public material
{
public:
	Texture* albedo;
public:
	lambertian(Texture* a) :albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
	{
		// render with lambertian lumination
		vec3 target = vec3(rec.p) + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.p, target - rec.p, r_in.time());
		attenuation = albedo->value(0,0,rec.p);
		return true;
	}
};

vec3 reflect(const vec3& v, const vec3& n)
{
	///
	return vec3(v) - vec3(2) * vec3(dot(v, n)) * n;
}

bool refract(const vec3& v, const vec3& n, double ni_over_nt, vec3& refracted)
{
	vec3 uv = unit_vector(v);
	double dt = dot(uv, n);
	double discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
	if (discriminant > 0)
	{
		refracted = vec3(ni_over_nt) * (uv - vec3(n) * vec3(dt)) - vec3(n) * vec3(sqrt(discriminant));
		return true;
	}
	else
		return false;
}

double schlick(double cosine, double ref_idx)
{
	double r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

class metal :public material
{
public:
	vec3 albedo;
	double fuzz;
public:
	metal(const vec3& a, double f) :albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
	{
		// reflection of mirror
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected+vec3(fuzz)*random_in_unit_sphere());
		attenuation = albedo;
		return dot(scattered.direction(), rec.normal) > 0;
	}
};

class dielectric :public material
{
public:
	double ref_idx;
public:
	dielectric(double ri) : ref_idx(ri){}
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const
	{
		vec3 outward_normal;
		vec3 reflected = reflect(r_in.direction(), rec.normal);
		double ni_over_nt;
		attenuation = vec3(0.5, 0.5, 0.5);
		vec3 refracted;
		double reflect_prob;
		double cosine;

		if (dot(r_in.direction(), rec.normal) > 0)
		{
			// the incident ray follows the direction of normal vector
			// cos(r_in)>0
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}
		else
		{
			// the incident ray is the contrary direction of normal vector
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
			cosine = -dot(r_in.direction(), rec.normal)/ r_in.direction().length();
		}
		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
		{
			// if refracted
			reflect_prob = schlick(cosine, ref_idx);
		}
		else
		{
			// if not
			scattered = ray(rec.p, reflected);
			reflect_prob = 1.0;
		}
		double rand_num = (double)(rand()) / RAND_MAX;
		if (rand_num < reflect_prob)
		{
			// reflect
			//scattered = ray(rec.p, reflected);	
		}
		else
		{
			// refract
			scattered = ray(rec.p, refracted);
		}
		return true;
	}
};