#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED

#include "transform/matrix.hpp"
#include "context.h"

namespace SoftwareRenderer {
namespace Transform {

	template <typename T>
	Matrix44<T> Translate(T x, T y, T z)
	{
		T arg[] = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			x, y, z, 1
		};
		return Matrix44<T>(arg);
	}

	template <typename T>
	Matrix44<T> AxisAngle(Vec3<T> axis, T angle)
	{
		T c = cos(angle);
		T s = sin(angle);
		T oneMinusCos = 1 - c;
		T arg[] = {
			oneMinusCos * axis.x * axis.x + c, oneMinusCos * axis.x * axis.y + axis.z * s, oneMinusCos * axis.x * axis.z - axis.y * s, 0,
			oneMinusCos * axis.x * axis.y - axis.z * s, oneMinusCos * axis.y * axis.y + c, oneMinusCos * axis.y * axis.z + axis.x * s, 0,
			oneMinusCos * axis.x * axis.z + axis.y * s, oneMinusCos * axis.y * axis.z - axis.x * s, oneMinusCos * axis.z * axis.z + c, 0,
			0, 0, 0, 1
		};
		return Matrix44<T>(arg);
	}

	template <typename T>
	void ToNDC(Vec4<T>& clip0, Vec4<T>& clip1, Vec4<T>& clip2)
	{
		clip0.x /= clip0.w;
		clip0.y /= clip0.w;
		clip0.z /= clip0.w;
		clip1.x /= clip1.w;
		clip1.y /= clip1.w;
		clip1.z /= clip1.w;
		clip2.x /= clip2.w;
		clip2.y /= clip2.w;
		clip2.z /= clip2.w;
	}

	template <typename T = int, typename U = float>
	void Viewport(T x, T y, T width, T height, U n, U f, Vec4<U>& ndc)
	{
		ndc.x = (ndc.x + 1) * width / 2 + x;
		ndc.y = (ndc.y + 1) * height / 2 + y;
		ndc.z = (ndc.z * (f - n) + (f + n)) / 2;
	}
}}

#endif