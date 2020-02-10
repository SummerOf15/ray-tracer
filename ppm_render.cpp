#include <iostream>
#include <fstream>
#include "vec3.h"
#include "ray.h"
#include "sphere.h"
#include "hitablelist.h"
#include "camera.h"
#include <random>
#include "materials.h"
#include <omp.h>
#include <time.h>
using namespace std;

double rand_num()
{
	return (double)(rand()) / RAND_MAX;
}
double hit_sphere(const vec3& center, double radius, const ray& r)
{
	// if the equation has 1 solution: tangent, 2 solutions: cross, 0 solution: disjoint.
	vec3 oc = r.origin() - center;
	double a = dot(r.direction(), r.direction());
	double b = 2.0 * dot(oc, r.direction());
	double c = dot(oc, oc) - radius * radius;
	double discriminant = b * b - 4 * a * c;
	// if there isn't solution, return 0
	// on the contrary, return the t
	if (discriminant < 0)
		return -1.0;
	else
		return (-b - (double)sqrt(discriminant)) / (2.0 * a);
}


vec3 color(const ray& r, Hitable *world, int depth)
{
	hit_record rec;
	// set the init max distance as 100000(the max)
	if (world->hit(r,0.00001,10000,rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth);
		}
		else
		{
			return vec3(0, 0, 0);
		}
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());// normalized y to (-1,1)
		double t = 0.5 * (unit_direction.y() + 1.0);// offset and scale to (0,1)
		// color blending
		return vec3(1.0 - t) * vec3(1.0, 1.0, 1.0) + vec3(t) * vec3(0.5, 0.7, 1.0);
	}
}

HitableList* random_scene()
{
	int n = 40;
	Hitable** list = new Hitable * [n + 1];
	Texture* checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));
	list[0] = new Sphere(vec3(0, -1000, 0), 1000, new lambertian(checker));
	int i = 1;
	for (int a = -3; a < 3; a++)
	{
		for (int b = -3; b < 3; b++)
		{
			double choose_mat = rand_num();
			vec3 center(a + 0.9 * rand_num(), 0.2, b + 0.9 * rand_num());
			if ((center - vec3(4, 0.2, 0)).length() > 0.9)
			{
				if (choose_mat < 0.8)
				{
					//diffuse
					Texture* t = new constant_texture(vec3(rand_num() * rand_num(), rand_num() * rand_num(), rand_num() * rand_num()));
					list[i++] = new MovingSphere(center, center + vec3(0, 0.5 * rand_num(), 0), 0, 1, 0.2, new lambertian(t));
				}
				else if(choose_mat<0.95)
				{
					//metal
					list[i++] = new Sphere(center, 0.2, 
						new metal(vec3(0.5 * (1.0 + rand_num()), 0.5 * (1.0 + rand_num()), 0.5 * (1.0 + rand_num())), 0.5 * rand_num()));

				}
				else
				{
					//glass
					list[i++] = new Sphere(center, 0.2, new dielectric(0.5));
				}
			}
		}
	}
	list[i++] = new Sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
	list[i++] = new Sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
	list[i++] = new Sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new HitableList(list, i);
}

int main()
{
	int nx = 300;
	int ny = 200;
	int ns = 10;
	ofstream outfile;
	outfile.open("render.ppm", ios::out);
	outfile << "P3\n" << nx << " " << ny << "\n255\n";
	cout << "P3\n" << nx << " " << ny << "\n255\n";
	clock_t start, finish;
	start = clock();

	//test example 2: various materials
	// a list to restore objects
	//Hitable* list[5];

	/*list[0] = new Sphere(vec3(0, 0, -1), 0.5, new lambertian(new constant_texture(vec3(0.1, 0.2, 0.5))));
	list[1] = new Sphere(vec3(0, -100.5, -1), 100, new lambertian(new constant_texture(vec3(0.8, 0.8, 0.0))));
	list[2] = new Sphere(vec3(-1, 0, 0), 0.5, new metal(vec3(0.7, 0.6, 0.5), 0.0));
	list[3] = new Sphere(vec3(-3, 0, 1), 0.5, new dielectric(1.5));
	list[4] = new Sphere(vec3(-3, 0, 1), -0.45, new dielectric(1.5));
	vec3 lookfrom(6, 2, 1);
	vec3 lookat(0, 0, 0);
	double dist_to_focus = 7;
	double aperture = 0.1;
	Camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, double(nx) / double(ny), aperture, dist_to_focus);*/


	// test example 1: sphere and moving sphere
	//------------------------------
	/*vec3 center(-2, 0.2, 1);
	Texture* checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));
	list[0] = new Sphere(vec3(0, -1000, 0), 1000, new lambertian(checker));
	Texture* t = new constant_texture(vec3(rand_num() * rand_num(), rand_num() * rand_num(), rand_num() * rand_num()));
	list[1] = new MovingSphere(center, center + vec3(0, 0.5 * rand_num(), 0), 0, 1, 0.2, new lambertian(t));
	list[2] = new Sphere(vec3(0, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
		
	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	double dist_to_focus = 10;
	double aperture = 0.1;
	Camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, (double)(nx) / (double)(ny), aperture, dist_to_focus,0.0,1.0);*/
	//---------------------------

	//Hitable* world = new HitableList(list, 5);
	// random scene
	
	// test examples: final result
	Hitable* world = random_scene();

	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	double dist_to_focus = 10;
	double aperture = 0.1;
	Camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, (double)(nx) / (double)(ny), aperture, dist_to_focus, 0.0, 1.0);
	
	vec3** pcol = new vec3 * [ny];
	for (int i = 0; i < ny; i++)
	{
		pcol[i] = new vec3[nx];
	}
//#pragma omp parallel for num_threads(3)
	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				double u = (double)(i+ rand_num()) / nx;
				double v = (double)(j+ rand_num()) / ny;
				ray r = cam.get_ray(u, v);
				// all the values should in range 0 and 255
				vec3 p = r.point_at_parameter(2.0);
				col += color(r, world,0);
			}
			col /= (double)(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));	
			pcol[ny - j - 1][i] = col;

		}
	}
	for (int j = 0; j < ny; j++)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 col = pcol[j][i];
			int ir = (int)(255.99 * col[0]);
			int ig = (int)(255.99 * col[1]);
			int ib = (int)(255.99 * col[2]);
			outfile << ir << " " << ig << " " << ib << "\n";
		}
	}
	outfile.close();
	finish = clock();
	delete[] pcol;
	cout << (double)(finish - start) / CLOCKS_PER_SEC << endl;
	return 0;
}