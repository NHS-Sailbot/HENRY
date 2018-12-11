#include "vec.h"



namespace Math
{

	/* VEC2 struct */

	vec2::vec2() : x(0.f), y(0.f) {}
	vec2::vec2(const float val) : x(val), y(val) {}
	vec2::vec2(const float _x, const float _y) : x(_x), y(_y) {}

	vec2 & operator+(vec2 & left, const vec2 & right) { return left.add(right); }
	vec2 & operator-(vec2 & left, const vec2 & right) { return left.sub(right); }
	vec2 & operator*(vec2 & left, const vec2 & right) { return left.mult(right); }
	vec2 & operator/(vec2 & left, const vec2 & right) { return left.div(right); }

	vec2 & vec2::operator+=(const vec2 & delta)	{ return add(delta); }
	vec2 & vec2::operator-=(const vec2 & delta)	{ return sub(delta); }
	vec2 & vec2::operator*=(const vec2 & delta)	{ return mult(delta); }
	vec2 & vec2::operator/=(const vec2 & delta)	{ return div(delta); }

	bool vec2::operator==(const vec2 & b)	{ return x == b.x && y == b.y; }

	vec2 & vec2::add(const vec2 & delta)
	{
		x += delta.x; y += delta.y;
		return *this;
	}
	vec2 & vec2::sub(const vec2 & delta)
	{
		x -= delta.x; y -= delta.y;
		return *this;
	}
	vec2 & vec2::mult(const vec2 & delta)
	{
		x *= delta.x; y *= delta.y;
		return *this;
	}
	vec2 & vec2::div(const vec2 & delta)
	{
		x /= delta.x; y /= delta.y;
		return *this;
	}

	/* VEC3 struct */

	vec3::vec3() : x(0.f), y(0.f), z(0.f) {}
	vec3::vec3(const float & val) : x(val), y(val), z(val) {}
	vec3::vec3(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z) {}

	vec3 & operator+(vec3 & left, const vec3 & right) { return left.add(right); }
	vec3 & operator-(vec3 & left, const vec3 & right) { return left.sub(right); }
	vec3 & operator*(vec3 & left, const vec3 & right) { return left.mult(right); }
	vec3 & operator/(vec3 & left, const vec3 & right) { return left.div(right); }

	vec3 & vec3::operator+=(const vec3 & delta) { return add(delta); }
	vec3 & vec3::operator-=(const vec3 & delta) { return sub(delta); }
	vec3 & vec3::operator*=(const vec3 & delta) { return mult(delta); }
	vec3 & vec3::operator/=(const vec3 & delta) { return div(delta); }

	bool vec3::operator==(const vec3 & b) { return x == b.x && y == b.y && z == b.z; }

	vec3 & vec3::add(const vec3 & delta)
	{
		x += delta.x; y += delta.y; z += delta.z;
		return *this;
	}
	vec3 & vec3::sub(const vec3 & delta)
	{
		x -= delta.x; y -= delta.y; z -= delta.z;
		return *this;
	}
	vec3 & vec3::mult(const vec3 & delta)
	{
		x *= delta.x; y *= delta.y; z *= delta.z;
		return *this;
	}
	vec3 & vec3::div(const vec3 & delta)
	{
		x /= delta.x; y /= delta.y; z /= delta.z;
		return *this;
	}

}