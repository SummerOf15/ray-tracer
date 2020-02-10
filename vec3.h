#pragma once
#include<math.h>
#include<stdlib.h>
#include<iostream>
using namespace std;

class vec3
{
public:
	double e[3];
public:
	vec3() {}
	vec3(double e0, double e1, double e2)
	{
		e[0] = e0;
		e[1] = e1;
		e[2] = e2;
	}
	vec3(double t)
	{
		e[0] = t;
		e[1] = t;
		e[2] = t;
	}
	inline double x() const { return e[0]; }
	inline double y() const { return e[1]; }
	inline double z() const { return e[2]; }
	inline double r() const { return e[0]; }
	inline double g() const { return e[1]; }
	inline double b() const { return e[2]; }

	inline const vec3& operator+() const { return *this; }
	inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	inline double operator[](int i) const { return e[i]; }
	inline double& operator[](int i) { return e[i]; }
	inline vec3 operator+(const vec3& v2)
	{
		return vec3(e[0] + v2.e[0], e[1] + v2.e[1], e[2] + v2.e[2]);
	}
	inline vec3 operator-(const vec3& v2)
	{
		return vec3(e[0] - v2.e[0], e[1] - v2.e[1], e[2] - v2.e[2]);
	}
	inline vec3 operator*(const vec3& v2)
	{
		return vec3(e[0] * v2.e[0], e[1] * v2.e[1], e[2] * v2.e[2]);
	}
	inline vec3 operator*(const double a)
	{
		return vec3(a * e[0], a * e[1], a * e[2]);
	}
	inline vec3 operator/(const vec3& v2)
	{
		return vec3(e[0] / v2.e[0], e[1] / v2.e[1], e[2] / v2.e[2]);
	}
	inline vec3 operator/(const double a)
	{
		return vec3(e[0] / a, e[1] / a, e[2] / a);
	}
	inline vec3& operator+=(const vec3& v2)
	{
		*this = *this + v2;
		return *this;
	}
	inline vec3& operator-=(const vec3& v2)
	{
		*this = *this - v2;
		return *this;
	}
	inline vec3& operator*=(const vec3& v2)
	{
		*this = *this * v2;
		return *this;
	}
	inline vec3& operator/=(const vec3& v2)
	{
		*this = *this / v2;
		return *this;
	}

	inline double length() const
	{
		return std::sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	}
	inline double squared_length() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}
	inline void make_unit_vector()
	{
		double k = 1.0 / this->length();
		e[0] *= k; e[1] *= k; e[2] *= k;
	}
};

inline istream& operator>>(istream& is, vec3& t)
{
	is >> t.e[0] >> t.e[1] >> t.e[2];
	return is;
}

inline ostream& operator<<(ostream& os, const vec3& t)
{
	os << t.e[0] << t.e[1] << t.e[2];
	return os;
}
inline double dot(const vec3& v1, const vec3& v2)
{
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}
inline vec3 cross(const vec3& v1, const vec3& v2)
{
	return vec3((v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]), -(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0]), (v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
}

inline vec3 unit_vector(vec3 v)
{
	return v / v.length();
}

inline vec3 operator-(const vec3& v1, const vec3& v2)
{
	return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}
inline vec3 operator+(const vec3& v1, const vec3& v2)
{
	return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}
inline vec3 operator*(const vec3& v1, const vec3& v2)
{
	return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}
