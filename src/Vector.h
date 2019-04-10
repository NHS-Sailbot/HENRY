#pragma once

namespace Math
{
template <typename T, int S>
struct Vector;

template <typename T>
struct Vector<T, 2>
{
	union {
		T x, r, s;
	};
	union {
		T y, g, t;
	};

	Vector<T, 2> &add(const Vector<T, 2> &vec);
	Vector<T, 2> &sub(const Vector<T, 2> &vec);
	Vector<T, 2> &mul(const T fac);
	Vector<T, 2> &div(const T fac);
	T dot(const Vector<T, 2> &vec) const;
	T smag() const;

	T &operator[](int i) const;

	Vector<T, 2> &operator+=(const Vector<T, 2> &vec);
	Vector<T, 2> &operator-=(const Vector<T, 2> &vec);
	Vector<T, 2> &operator*=(const T fac);
	Vector<T, 2> &operator/=(const T fac);

	Vector<T, 2> &operator+(const Vector<T, 2> &vec);
	Vector<T, 2> &operator-(const Vector<T, 2> &vec);
	Vector<T, 2> &operator*(const T fac);
	Vector<T, 2> &operator/(const T fac);
};

template <typename T>
struct Vector<T, 3>
{
	union {
		T x, r, s;
	};
	union {
		T y, g, t;
	};
	union {
		T z, b, p;
	};

	Vector<T, 3> &add(const Vector<T, 3> &vec);
	Vector<T, 3> &sub(const Vector<T, 3> &vec);
	T dot(const Vector<T, 3> &vec) const;
	Vector<T, 3> &cross(const Vector<T, 3> &vec);
	T smag() const;

	T &operator[](int i) const;

	Vector<T, 3> &operator+=(const Vector<T, 3> &vec);
	Vector<T, 3> &operator-=(const Vector<T, 3> &vec);

	Vector<T, 3> &operator+(const Vector<T, 3> &vec);
	Vector<T, 3> &operator-(const Vector<T, 3> &vec);
};

template <typename T>
struct Vector<T, 4>
{
	union {
		T x, r, s;
	};
	union {
		T y, g, t;
	};
	union {
		T z, b, p;
	};
	union {
		T w, a, q;
	};

	Vector<T, 4> &add(const Vector<T, 4> &vec);
	Vector<T, 4> &sub(const Vector<T, 4> &vec);
	T dot(const Vector<T, 4> &vec) const;
	T smag() const;

	T &operator[](int i) const;

	Vector<T, 4> &operator+=(const Vector<T, 4> &vec);
	Vector<T, 4> &operator-=(const Vector<T, 4> &vec);

	Vector<T, 4> &operator+(const Vector<T, 4> &vec);
	Vector<T, 4> &operator-(const Vector<T, 4> &vec);
};

typedef Vector<float, 2> vec2;
typedef Vector<float, 3> vec3;
typedef Vector<float, 4> vec4;

typedef Vector<double, 2> dvec2;
typedef Vector<double, 3> dvec3;
typedef Vector<double, 4> dvec4;

typedef Vector<unsigned char, 2> cvec2;
typedef Vector<unsigned char, 3> cvec3;
typedef Vector<unsigned char, 4> cvec4;

typedef Vector<short, 2> svec2;
typedef Vector<short, 3> svec3;
typedef Vector<short, 4> svec4;

typedef Vector<int, 2> ivec2;
typedef Vector<int, 3> ivec3;
typedef Vector<int, 4> ivec4;
} // namespace Math