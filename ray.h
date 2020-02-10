#pragma once
#ifndef RAYH
#define RAYH
#include "vec3.h"

class ray
{
public:
	vec3 A;// the original point
	vec3 B;// the direction
	double _time; // time for the ray->moving objects
public:
	ray() {}
	ray(const vec3& a, const vec3& b) :A(a), B(b) {};
	ray(const vec3& a, const vec3& b, double time) :A(a), B(b), _time(time) {};
	vec3 origin() const { return A; }
	vec3 direction() const { return B; }
	vec3 point_at_parameter(double t) const { return vec3(A) + vec3(t) * B; }
	double time() const { return _time; }
};
#endif // !RAYH
