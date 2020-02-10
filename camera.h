#pragma once
#ifndef CAMERAH
#define CAMERAH

#include "ray.h"
#define PI 3.1415926

vec3 random_in_unit_disk()
{
	// random number in(-1,1)
	vec3 p;
	do
	{
		p = vec3(2.0) * vec3(double(rand()) / RAND_MAX, double(rand()) / RAND_MAX, 0) - vec3(1, 1, 0);
	} while (dot(p, p) >= 1.0);
	return p;
}

class Camera 
{
public:
	vec3 origin;
	vec3 lower_left_corner;
	vec3 horizontal;
	vec3 vertical;
	double lens_radius;
	vec3 u, v, w;
	double time0, time1; // variables for shutter opening and closing
public:
	Camera()
	{
		lower_left_corner = vec3(-2.0, -1.0, -1.0);
		horizontal = vec3(4.0, 0.0, 0.0);
		vertical = vec3(0.0, 2.0, 0.0);
		origin = vec3(0.0, 0.0, 0.0);
	}
	Camera(vec3 lookfrom, vec3 lookat, vec3 vup, double vfov, double aspect)
	{
		double theta = vfov * PI / 180;
		double half_height = tan(theta / 2);
		double half_width = aspect * half_height;
		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		//lower_left_corner = vec3(-half_width, -half_height, -1.0);
		lower_left_corner = origin - vec3(half_width) * u - vec3(half_height) * v - w;
		horizontal = vec3(2) * vec3(half_width) * u;
		vertical = vec3(2) * vec3(half_height) * v;
	}
	Camera(vec3 lookfrom, vec3 lookat, vec3 vup, double vfov, double aspect, double aperture, double focus_dist)
	{
		lens_radius = aperture / 2;
		double theta = vfov * PI / 180;
		double half_height = tan(theta / 2);
		double half_width = aspect * half_height;
		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		lower_left_corner = origin - vec3(half_width * focus_dist) * u - vec3(half_height * focus_dist) * v - vec3(focus_dist) * w;
		horizontal = vec3(half_width * focus_dist) * u * 2;
		vertical = vec3(half_height * focus_dist) * v * 2;
	}
	Camera(vec3 lookfrom, vec3 lookat, vec3 vup, double vfov, double aspect, double aperture, double focus_dist, double t0, double t1)
	{
		time0 = t0;
		time1 = t1;
		lens_radius = aperture / 2;
		double theta = vfov * PI / 180;
		double half_height = tan(theta / 2);
		double half_width = aspect * half_height;
		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);// towards insides
		u = unit_vector(cross(vup, w));// horizontal
		v = cross(w, u);// vertical
		lower_left_corner = origin - u * (half_width * focus_dist) - v * (half_height * focus_dist) - w * focus_dist;
		horizontal = u * (2 * half_width * focus_dist);
		vertical = v * (2 * half_height * focus_dist);
	}

	ray get_ray(double s, double t)
	{
		vec3 rd = random_in_unit_disk() * lens_radius;
		vec3 offset = u * rd.x() + v * rd.y();
		double time = time0 + double(rand()) / RAND_MAX * (time1 - time0);
		return ray(origin + offset, lower_left_corner + horizontal * s + vertical * t - origin - offset, time);
	}
};
#endif
