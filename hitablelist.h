#pragma once
#ifndef HITABLELISTH
#define HITABLELISTH

#include "hitable.h"

// A class to record a list of objects and check the hit status
class HitableList : public Hitable
{
public:
	Hitable** list;
	int list_size;
public:
	HitableList(){}
	HitableList(Hitable** l, int n) :list(l), list_size(n) {}
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const
	{
		hit_record temp_rec;
		bool hit_anything = false;//store all the hit records
		double closest_so_far = t_max;
		for (int i = 0; i < list_size; i++)
		{
			if (list[i]->hit(r, t_min, closest_so_far, temp_rec))
			{
				hit_anything = true;
				closest_so_far = temp_rec.t;
				rec = temp_rec;
			}
		}
		return hit_anything;
	}
	virtual bool bounding_box(float t0, float t1, Overlap& box) const
	{
		if (list_size < 1) return false;
		Overlap temp_box;
		bool first_true = list[0]->bounding_box(t0, t1, temp_box);
		if (!first_true)
			return false;
		else
			box = temp_box;
		for (int i = 1; i < list_size; i++)
		{
			if (list[0]->bounding_box(t0, t1, temp_box))
			{
				box = surrounding_box(box, temp_box);
			}
			else
				return false;
		}
		return true;
	}
};
#endif // !HITABLELISTH
