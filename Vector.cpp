#include "Vector.h"

/* This is not going to be documented, it's source 
from one of my personal libraries that adds math
macros, functions and structs. */

namespace Math
{
/*--------------------------------
|                                |
|            Vector2             |
|                                |
--------------------------------*/

// ---- Member functions (Methods) ----

// 'add' adds each of the given vector's members
// to the owner's members and returns a reference
// to the newly updated owner
template <typename T>
Vector<T, 2> &Vector<T, 2>::add(const Vector<T, 2> &vec)
{
	x += vec.x;
	y += vec.y;
	return *this;
}

// 'sub' subtracts each of the given vector's members
// from the owner's members and returns a reference
// to the newly updated owner
template <typename T>
Vector<T, 2> &Vector<T, 2>::sub(const Vector<T, 2> &vec)
{
	x -= vec.x;
	y -= vec.y;
	return *this;
}

// 'mul' multiplies the components of the vector
// by a constant and returns a reference to the
// newly updated owner
template <typename T>
Vector<T, 2> &Vector<T, 2>::mul(const T fac)
{
	x *= fac;
	y *= fac;
	return *this;
}

// 'div' divides the components of the vector
// by a constant and returns a reference to the
// newly updated owner
template <typename T>
Vector<T, 2> &Vector<T, 2>::div(const T fac)
{
	T one_over_fac = 1.f / fac;
	x *= one_over_fac;
	y *= one_over_fac;
	return *this;
}

// 'dot' computes the dot product of this vector
// with the provided one and returns the value
template <typename T>
T Vector<T, 2>::dot(const Vector<T, 2> &vec) const
{
	return x * vec.x + y * vec.y;
}

// 'smag' calculates the length of the vector without
// the final step square root which is the most
// computationally intensive step. This is intended
// to be used when the square root is not needed, such
// as in comparisons where the opposing bound is fixed
// or can be squared. ex: (myVec.smag() > SQ(2))
template <typename T>
T Vector<T, 2>::smag() const
{
	return x * x + y * y;
}

// ---- Operators ----

// operator[] returns the indexed value (i) of the vector
template <typename T>
T &Vector<T, 2>::operator[](int i) const
{
	// TODO: impliment checking for memory access violations
	// for i < 0 || i > 1 when in debugging mode. This error
	// checking should be removed at compile time when releasing
	// the software as it is unneeded overhead.
	return *((T *)this + i);
}

// operator+=
template <typename T>
Vector<T, 2> &Vector<T, 2>::operator+=(const Vector<T, 2> &vec)
{
	return add(vec);
}

// operator-=
template <typename T>
Vector<T, 2> &Vector<T, 2>::operator-=(const Vector<T, 2> &vec)
{
	return sub(vec);
}

// operator*=
template <typename T>
Vector<T, 2> &Vector<T, 2>::operator*=(const T fac)
{
	return mul(fac);
}

// operator/=
template <typename T>
Vector<T, 2> &Vector<T, 2>::operator/=(const T fac)
{
	return div(fac);
}

// These next operators perform copy because it takes two constant
// vectors and creates a new one without modifying the originals.
// ----------------------------------------------------------------
//     example:
//
//  |  Vector2 a(1.f, 2.f);
//  |  Vector2 b(7.f, 3.f);
//  |  std::cout << (a + b).x << '\n';
//
//     one would expect a and b to be unchanged and thus a
//     copy is required.
// ----------------------------------------------------------------

// operator+
template <typename T>
Vector<T, 2> &Vector<T, 2>::operator+(const Vector<T, 2> &vec)
{
	return Vector<T, 2>(*this).add(vec);
}

// operator-
template <typename T>
Vector<T, 2> &Vector<T, 2>::operator-(const Vector<T, 2> &vec)
{
	return Vector<T, 2>(*this).sub(vec);
}

// operator*
template <typename T>
Vector<T, 2> &Vector<T, 2>::operator*(const T fac)
{
	return Vector<T, 2>(*this).mul(fac);
}

// operator/
template <typename T>
Vector<T, 2> &Vector<T, 2>::operator/(const T fac)
{
	return Vector<T, 2>(*this).div(fac);
}

/*--------------------------------
|                                |
|            Vector3             |
|                                |
--------------------------------*/

// ---- Member functions (Methods) ----

// 'add' adds each of the given vector's members
// to the owner's members and returns a reference
// to the newly updated owner
template <typename T>
Vector<T, 3> &Vector<T, 3>::add(const Vector<T, 3> &vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;
}

// 'sub' subtracts each of the given vector's members
// from the owner's members and returns a reference
// to the newly updated owner
template <typename T>
Vector<T, 3> &Vector<T, 3>::sub(const Vector<T, 3> &vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

// 'dot' computes the dot product of this vector
// with the provided one and returns the value
template <typename T>
T Vector<T, 3>::dot(const Vector<T, 3> &vec) const
{
	return x * vec.x + y * vec.y + z * vec.z;
}

// 'cross' applies the cross product of this vector
// and the provided vector, changing this vector and
// returning a reference to this
template <typename T>
Vector<T, 3> &Vector<T, 3>::cross(const Vector<T, 3> &vec)
{
	T tx = x, ty = y;
	x = y * vec.z - z * vec.y;
	y = z * vec.x - tx * vec.z;
	z = tx * vec.y - ty * vec.x;
	return *this;
}

// 'smag' calculates the length of the vector without
// the final step square root which is the most
// computationally intensive step.
template <typename T>
T Vector<T, 3>::smag() const
{
	return x * x + y * y + z * z;
}

// ---- Operators ----

// operator[] returns the indexed value (i) of the vector
template <typename T>
T &Vector<T, 3>::operator[](int i) const
{
	// TODO: impliment checking for memory access violations
	// for i < 0 || i > 1 when in debugging mode. This error
	// checking should be removed at compile time when releasing
	// the software as it is unneeded overhead.
	return *((T *)this + i);
}

// operator+=
template <typename T>
Vector<T, 3> &Vector<T, 3>::operator+=(const Vector<T, 3> &vec)
{
	return add(vec);
}

// operator-=
template <typename T>
Vector<T, 3> &Vector<T, 3>::operator-=(const Vector<T, 3> &vec)
{
	return sub(vec);
}

// These next operators perform copy because it takes two constant
// vectors and creates a new one without modifying the originals.

// operator+
template <typename T>
Vector<T, 3> &Vector<T, 3>::operator+(const Vector<T, 3> &vec)
{
	return Vector<T, 3>(*this).add(vec);
}

// operator-
template <typename T>
Vector<T, 3> &Vector<T, 3>::operator-(const Vector<T, 3> &vec)
{
	return Vector<T, 3>(*this).sub(vec);
}

/*--------------------------------
|                                |
|            Vector4             |
|                                |
--------------------------------*/

// ---- Member functions (Methods) ----

// 'add' adds each of the given vector's members
// to the owner's members and returns a reference
// to the newly updated owner
template <typename T>
Vector<T, 4> &Vector<T, 4>::add(const Vector<T, 4> &vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	w += vec.w;
	return *this;
}

// 'sub' subtracts each of the given vector's members
// from the owner's members and returns a reference
// to the newly updated owner
template <typename T>
Vector<T, 4> &Vector<T, 4>::sub(const Vector<T, 4> &vec)
{
	y -= vec.y;
	z -= vec.z;
	w -= vec.w;
	return *this;
}

// 'dot' computes the dot product of this vector
// with the provided one and returns the value
template <typename T>
T Vector<T, 4>::dot(const Vector<T, 4> &vec) const
{
	return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
}

// 'smag' calculates the length of the vector without
// the final step square root which is the most
// computationally intensive step
template <typename T>
T Vector<T, 4>::smag() const
{
	return x * x + y * y + z * z + w * w;
}

// ---- Operators ----

// operator[] returns the indexed value (i) of the vector
template <typename T>
T &Vector<T, 4>::operator[](int i) const
{
	// TODO: impliment checking for memory access violations
	// for i < 0 || i > 1 when in debugging mode. This error
	// checking should be removed at compile time when releasing
	// the software as it is unneeded overhead.
	return *((T *)this + i);
}

// operator+=
template <typename T>
Vector<T, 4> &Vector<T, 4>::operator+=(const Vector<T, 4> &vec)
{
	return add(vec);
}

// operator-=
template <typename T>
Vector<T, 4> &Vector<T, 4>::operator-=(const Vector<T, 4> &vec)
{
	return sub(vec);
}

// These next operators perform copy because it takes two constant
// vectors and creates a new one without modifying the originals.

// operator+
template <typename T>
Vector<T, 4> &Vector<T, 4>::operator+(const Vector<T, 4> &vec)
{
	return Vector<T, 4>(*this).add(vec);
}

// operator-
template <typename T>
Vector<T, 4> &Vector<T, 4>::operator-(const Vector<T, 4> &vec)
{
	return Vector<T, 4>(*this).sub(vec);
}

template struct Vector<float, 2>;
template struct Vector<float, 3>;
template struct Vector<float, 4>;

template struct Vector<double, 2>;
template struct Vector<double, 3>;
template struct Vector<double, 4>;

template struct Vector<unsigned char, 2>;
template struct Vector<unsigned char, 3>;
template struct Vector<unsigned char, 4>;

template struct Vector<short, 2>;
template struct Vector<short, 3>;
template struct Vector<short, 4>;

template struct Vector<int, 2>;
template struct Vector<int, 3>;
template struct Vector<int, 4>;

} // namespace Math