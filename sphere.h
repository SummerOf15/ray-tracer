#pragma once
#ifndef SPHEREH
#define SPHEREH

#include"hitable.h"

class Sphere :public Hitable
{
public:
	vec3 center;
	double radius;
	material* mat_ptr;
public:
	Sphere(vec3 cen,double r, material* mat):center(cen),radius(r),mat_ptr(mat){}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const
	{
		// if the equation has 1 solution: tangent, 2 solutions: cross, 0 solution: disjoint.
		vec3 oc = r.origin() - center;
		double a = dot(r.direction(), r.direction());
		double b = 2.0 * dot(oc, r.direction());
		double c = dot(oc, oc) - radius * radius;
		double discriminant = b * b - 4 * a * c;
		// if there isn't solution, return 0
		// on the contrary, return the t
		if (discriminant >= 0)
		{
			// case 1
			double temp = (-b - (double)sqrt(discriminant)) / (2.0 * a);
			if (temp<t_max && temp>t_min)
			{
				rec.t = temp;
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = (rec.p - center) / radius;
				rec.mat_ptr = mat_ptr;
				return true;
			}
			// case 2
			temp = (-b + (double)sqrt(discriminant))/ (2.0 * a);
			if (temp<t_max && temp>t_min)
			{
				rec.t = temp;
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = (rec.p - center) / radius;
				rec.mat_ptr = mat_ptr;
				return true;
			}
		}
		
		// if the intersection point is out of windows
		// if there is no solutions
		return false;
	}
	virtual bool bounding_box(float t0, float t1, Overlap& box) const
	{
		box = Overlap(center - vec3(radius, radius, radius), center + vec3(radius, radius, radius));
		return true;
	}
};
class MovingSphere :public Hitable
{
public:
	vec3 center0, center1;
	double time0, time1;
	double radius;
	material* mat_ptr;

public:
	MovingSphere(vec3 cen0, vec3 cen1, double t0, double t1, double r, material* m) :
		center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m) {};

	vec3 center(double time) const
	{
		// calculate the center position according to the time
		return (center1 - center0) * ((time - time0) / (time1 - time0)) + center0;
	}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const
	{
		// if the equation has 1 solution: tangent, 2 solutions: cross, 0 solution: disjoint.
		vec3 oc = r.origin() - center(r.time());
		double a = dot(r.direction(), r.direction());
		double b = 2.0 * dot(oc, r.direction());
		double c = dot(oc, oc) - radius * radius;
		double discriminant = b * b - 4 * a * c;
		// if there isn't solution, return 0
		// on the contrary, return the t
		if (discriminant >= 0)
		{
			// case 1
			double temp = (-b - (double)sqrt(discriminant)) / (2.0 * a);
			if (temp<t_max && temp>t_min)
			{
				rec.t = temp;
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = (rec.p - center(r.time())) / radius;
				rec.mat_ptr = mat_ptr;
				return true;
			}
			// case 2
			temp = (-b + (double)sqrt(discriminant)) / (2.0 * a);
			if (temp<t_max && temp>t_min)
			{
				rec.t = temp;
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = (rec.p - center(r.time())) / radius;
				rec.mat_ptr = mat_ptr;
				return true;
			}
		}

		// if the intersection point is out of windows
		// if there is no solutions
		return false;
	}
	virtual bool bounding_box(float t0, float t1, Overlap& box) const
	{
		box = Overlap(center0 - vec3(radius, radius, radius), center0 + vec3(radius, radius, radius));
		return true;
	}
};

#endif // !SPHEREH
