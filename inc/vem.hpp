#pragma once

#include <cmath>
#include <stdexcept>

namespace vem{

class vec2;
class vec3;
class vec4;
class mat3;
class mat4;

class vec2{
	public:
		float	x, y;

		vec2(void);
		vec2(const float &f);
		vec2(const float &fx, const float &fy);
		vec2(const vec2 &v);

		void	operator+=(const vec2 &v);
		void	operator-=(const vec2 &v);
		float	operator[](int i);
};

class vec3{
	public:
		float	x, y, z;

		vec3(void);
		vec3(const float &f);
		vec3(const float &fx, const float &fy, const float &fz);
		vec3(const vec3 &v);
		vec3(const mat4 &m);

		void	operator+=(const vec3 &v);
		void	operator-=(const vec3 &v);
		float	operator[](int i);
};

class vec4{
	public:
		float	x, y, z, a;

		vec4(void);
		vec4(const float &f);
		vec4(const float &fx, const float &fy, const float &fz, const float &fa);
		vec4(const vec4 &v);
		vec4(const vec3 &v, const float &f);

		void	operator+=(const vec4 &v);
		void	operator-=(const vec4 &v);
		float	operator[](int i);

};

class mat3{
	public:
		float	mat[3][3];

		mat3(void);
		mat3(const float &f);
		mat3(const float &f1, const float &f2, const float &f3);
		mat3(const vec3 &v1, const vec3 &v2, const vec3 &v3);
};

class mat4{
	public:
		float	mat[4][4];

		mat4(void);
		mat4(const float &f);
		mat4(const float &f1, const float &f2, const float &f3, const float &f4);
		mat4(const vec4 &v1, const vec4 &v2, const vec4 &v3, const vec4 &v4);
	
		mat4	operator*(const mat4 &m);
		mat4	operator=(const mat3 &m);

};

vec2	operator+(const vec2 &v1, const vec2 &v2);
vec3	operator+(const vec3 &v1, const vec3 &v2);
vec4	operator+(const vec4 &v1, const vec4 &v2);
vec2	operator-(const vec2 &v1, const vec2 &v2);
vec3	operator-(const vec3 &v1, const vec3 &v2);
vec4	operator-(const vec4 &v1, const vec4 &v2);
vec2	operator*(const vec2 &v1, const vec2 &v2);
vec3	operator*(const vec3 &v1, const vec3 &v2);
vec4	operator*(const vec4 &v1, const vec4 &v2);
mat4	operator*(const mat4 &m, const vec4 &v);
vec2	operator/(const vec2 &v1, const vec2 &v2);
vec3	operator/(const vec3 &v1, const vec3 &v2);
vec4	operator/(const vec4 &v1, const vec4 &v2);
bool	operator==(const vec2 &v1, const vec2 &v2);
bool	operator==(const vec3 &v1, const vec3 &v2);
bool	operator==(const vec4 &v1, const vec4 &v2);

float	dot(const vec2 &v1, const vec2 &v2);
float	dot(const vec3 &v1, const vec3 &v2);
vec2	normalize(const vec2 &v);
vec3	normalize(const vec3 &v);
float	mod(float value, float divisor);
float	radians(float degrees);
mat4	rotate(const mat4 &m, float angle, const vec3 &axis);

template <typename T>
T		clamp(T value, T minVal, T maxVal){
	if (minVal > maxVal)
		std::swap(minVal, maxVal);
	if (value < minVal)
		return (minVal);
	else if (value > maxVal)
		return (maxVal);
	else
		return (value);
}

}
