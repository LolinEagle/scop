#include <vem.hpp>

namespace vem{

vec2::vec2(void): x(0.f), y(0.f){
}

vec2::vec2(const float &f): x(f), y(f){
}

vec2::vec2(const float &fx, const float &fy): x(fx), y(fy){
}

vec2::vec2(const vec2 &v): x(v.x), y(v.y){
}

void	vec2::operator+=(const vec2 &v){
	x += v.x;
	y += v.y;
}

void	vec2::operator-=(const vec2 &v){
	x -= v.x;
	y -= v.y;
}

float	vec2::operator[](int i){
	if (i == 0) return (x);
	else if (i == 1) return (y);
	throw (std::runtime_error("index is past the end of the vector"));
}

vec3::vec3(void): x(0.f), y(0.f), z(0.f){
}

vec3::vec3(const float &f): x(f), y(f), z(f){
}

vec3::vec3(const float &fx, const float &fy, const float &fz): x(fx), y(fy), z(fz){
}

vec3::vec3(const vec3 &v): x(v.x), y(v.y), z(v.z){
}

vec3::vec3(const vec4 &v): x(v.x), y(v.y), z(v.z){
}

vec3::vec3(const mat4 &m): x(m.mat[0][0]), y(m.mat[0][1]), z(m.mat[0][2]){
}

void	vec3::operator+=(const vec3 &v){
	x += v.x;
	y += v.y;
	z += v.z;
}

void	vec3::operator-=(const vec3 &v){
	x -= v.x;
	y -= v.y;
	z -= v.z;
}

float	vec3::operator[](int i){
	if (i == 0) return (x);
	else if (i == 1) return (y);
	else if (i == 2) return (z);
	throw (std::runtime_error("index is past the end of the vector"));
}

vec4::vec4(void): x(0.f), y(0.f), z(0.f), a(0.f){
};

vec4::vec4(const float &f): x(f), y(f), z(f), a(f){
};

vec4::vec4(const float &fx, const float &fy, const float &fz, const float &fa)
: x(fx), y(fy), z(fz), a(fa){
}

vec4::vec4(const vec4 &v): x(v.x), y(v.y), z(v.z), a(v.a){
}

vec4::vec4(const vec3 &v, const float &f): x(v.x), y(v.y), z(v.z), a(f){
}

void	vec4::operator+=(const vec4 &v){
	x += v.x;
	y += v.y;
	z += v.z;
	a += v.a;
}

void	vec4::operator-=(const vec4 &v){
	x -= v.x;
	y -= v.y;
	z -= v.z;
	a -= v.a;
}

float	vec4::operator[](int i){
	if (i == 0) return (x);
	else if (i == 1) return (y);
	else if (i == 2) return (z);
	else if (i == 3) return (a);
	throw (std::runtime_error("index is past the end of the vector"));
}

mat3::mat3(void){
	for (int x = 0; x < 3; x++){
		for (int y = 0; y < 3; y++)
			mat[x][y] = 0.f;
	}
}

mat3::mat3(const float &f){
	for (int x = 0; x < 3; x++){
		for (int y = 0; y < 3; y++)
			mat[x][y] = f;
	}
}

mat3::mat3(const vec3 &v1, const vec3 &v2, const vec3 &v3){
	mat[0][0] = v1.x;
	mat[0][1] = v1.y;
	mat[0][2] = v1.z;
	mat[1][0] = v2.x;
	mat[1][1] = v2.y;
	mat[1][2] = v2.z;
	mat[2][0] = v3.x;
	mat[2][1] = v3.y;
	mat[2][2] = v3.z;
}

mat4::mat4(void){
	for (int y = 0; y < 4; y++){
		for (int x = 0; x < 4; x++)
			mat[y][x] = 0.f;
	}
}

mat4::mat4(const float &f){
	for (int y = 0; y < 4; y++){
		for (int x = 0; x < 4; x++)
			mat[y][x] = f;
	}
}

mat4::mat4(const vec4 &v1, const vec4 &v2, const vec4 &v3, const vec4 &v4){
	mat[0][0] = v1.x;
	mat[0][1] = v1.y;
	mat[0][2] = v1.z;
	mat[0][3] = v1.a;
	mat[1][0] = v2.x;
	mat[1][1] = v2.y;
	mat[1][2] = v2.z;
	mat[1][3] = v2.a;
	mat[2][0] = v3.x;
	mat[2][1] = v3.y;
	mat[2][2] = v3.z;
	mat[2][3] = v3.a;
	mat[3][0] = v4.x;
	mat[3][1] = v4.y;
	mat[3][2] = v4.z;
	mat[3][3] = v4.a;
}

mat4	mat4::operator*(const mat4 &m){
	mat4	ret;
	for (int y = 0; y < 4; y++){
		for (int x = 0; x < 4; x++)
			ret.mat[y][x] = mat[y][x] * m.mat[y][x];
	}
	return (ret);
}

mat4	mat4::operator=(const mat3 &m){
	mat4	ret;
	for (int y = 0; y < 4; y++){
		for (int x = 0; x < 4; x++){
			if (x < 3 && y < 3) ret.mat[y][x] = m.mat[y][x];
			else ret.mat[y][x] = 0.f;
		}
	}
	ret.mat[3][3] = 1.f;
	return (ret);
}

vec2	operator+(const vec2 &v1, const vec2 &v2){
	return (vec2(v1.x + v2.x, v1.y + v2.y));
}

vec3	operator+(const vec3 &v1, const vec3 &v2){
	return (vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z));
}

vec4	operator+(const vec4 &v1, const vec4 &v2){
	return (vec4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.a + v2.a));
}

vec2	operator-(const vec2 &v1, const vec2 &v2){
	return (vec2(v1.x - v2.x, v1.y - v2.y));
}

vec3	operator-(const vec3 &v1, const vec3 &v2){
	return (vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z));
}

vec4	operator-(const vec4 &v1, const vec4 &v2){
	return (vec4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.a - v2.a));
}

vec2	operator*(const vec2 &v1, const vec2 &v2){
	return (vec2(v1.x * v2.x, v1.y * v2.y));
}

vec3	operator*(const vec3 &v1, const vec3 &v2){
	return (vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z));
}

vec4	operator*(const vec4 &v1, const vec4 &v2){
	return (vec4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.a * v2.a));
}

vec2	operator/(const vec2 &v1, const vec2 &v2){
	return (vec2(v1.x / v2.x, v1.y / v2.y));
}

vec3	operator/(const vec3 &v1, const vec3 &v2){
	return (vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z));
}

vec4	operator/(const vec4 &v1, const vec4 &v2){
	return (vec4(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.a / v2.a));
}

bool	operator==(const vec2 &v1, const vec2 &v2){
	return (v1.x == v2.x && v1.y == v2.y ? true : false);
}

bool	operator==(const vec3 &v1, const vec3 &v2){
	return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z ? true : false);
}

bool	operator==(const vec4 &v1, const vec4 &v2){
	return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.a == v2.a ? true : false);
}

float	dot(const vec2 &v1, const vec2 &v2){
	return (v1.x * v2.x + v1.y * v2.y);
}

float	dot(const vec3 &v1, const vec3 &v2){
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

vec2	normalize(const vec2 &v){
	float	magnitude = sqrt(v.x * v.x + v.y * v.y);
	if (magnitude == 0.f)
		return {0.f, 0.f};
	return {v.x / magnitude, v.y / magnitude};
}

vec3	normalize(const vec3 &v){
	float	magnitude = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if (magnitude == 0.f)
		return {0.f, 0.f, 0.f};
	return {v.x / magnitude, v.y / magnitude, v.z / magnitude};
}

float	mod(float value, float divisor){
	if (divisor == 0.f)
		return (0.f);
	float	remainder = fmod(value, divisor);
	if ((remainder < 0.f && divisor > 0.f) || (remainder > 0.f && divisor < 0.f))
		remainder += divisor;
	return (remainder);
}

float	radians(float degrees){
	return (degrees * (M_PI / 180.f));
}

mat4	rotate(const mat4 &m, float angle, const vec3 &axis){
	vec3	normalizedAxis = normalize(axis);
	float	r = radians(angle);
	float	c = cos(r);
	float	s = sin(r);
	mat3	m3 = mat3(
		vec3(0.f, -normalizedAxis.z, normalizedAxis.y),
		vec3(normalizedAxis.z, 0.f, -normalizedAxis.x),
		vec3(-normalizedAxis.y, normalizedAxis.x, 0.f)
	);
	mat4	rotationMatrix = mat4(
		vec4(c + s * m3.mat[0][0], c + s * m3.mat[1][0], c + s * m3.mat[2][0], 0.f),
		vec4(c + s * m3.mat[0][1], c + s * m3.mat[1][1], c + s * m3.mat[2][1], 0.f),
		vec4(c + s * m3.mat[0][2], c + s * m3.mat[1][2], c + s * m3.mat[2][2], 0.f),
		vec4(0.f, 0.f, 0.f, 1.f)
	);
	return (rotationMatrix * m);
}

}
