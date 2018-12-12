#pragma once

namespace Math
{
	
	struct vec2
	{
		float x, y;

		vec2();
		vec2(const float _val);
		vec2(const float _x, const float _y);

		vec2 & add(const vec2 & delta);
		vec2 & sub(const vec2 & delta);
		vec2 & mult(const vec2 & delta);
		vec2 & div(const vec2 & delta);

		friend vec2 & operator+(vec2 & left, const vec2 & right);
		friend vec2 & operator-(vec2 & left, const vec2 & right);
		friend vec2 & operator*(vec2 & left, const vec2 & right);
		friend vec2 & operator/(vec2 & left, const vec2 & right);

		vec2 & operator+=(const vec2 & delta);
		vec2 & operator-=(const vec2 & delta);
		vec2 & operator*=(const vec2 & delta);
		vec2 & operator/=(const vec2 & delta);

		bool operator==(const vec2 & b);
	};

	struct vec3
	{
		float x, y, z;

		vec3();
		vec3(const float & val);
		vec3(const float _x, const float _y, const float _z);

		vec3 & add(const vec3 & delta);
		vec3 & sub(const vec3 & delta);
		vec3 & mult(const vec3 & delta);
		vec3 & div(const vec3 & delta);

		friend vec3 & operator+(vec3 & left, const vec3 & right);
		friend vec3 & operator-(vec3 & left, const vec3 & right);
		friend vec3 & operator*(vec3 & left, const vec3 & right);
		friend vec3 & operator/(vec3 & left, const vec3 & right);

		vec3 & operator+=(const vec3 & delta);
		vec3 & operator-=(const vec3 & delta);
		vec3 & operator*=(const vec3 & delta);
		vec3 & operator/=(const vec3 & delta);

		bool operator==(const vec3 & b);
	};
}