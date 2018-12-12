#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include "geometry/type.h"
#include "geometry/vec3.hpp"
#include "geometry/vec4.hpp"

#include <vector>
#include <array>
#include <iostream>

namespace SoftwareRenderer {
namespace Transform
{
	using SoftwareRenderer::Geometry::Vec3;
	using SoftwareRenderer::Geometry::Vec4;
	using std::ostream;
	using std::endl;

	// Column Major
	template<typename T>
    class Matrix44 {
    private:
        //static const int SIZE = 16;
		T _elems[4][4] = { {0, 0, 0, 0}, {0, 0 ,0 , 0}, {0, 0, 0, 0}, {0, 0, 0, 0} };
    public:
		Matrix44() {}
		Matrix44(T s)
		{
			for (uint8_t i = 0; i < 4; i++) {
				for (uint8_t j = 0; j < 4; j++) {
					_elems[i][j] = (i == j) * s;
				}
			}
		}
		Matrix44(const Matrix44& m)
		{
			for (uint8_t i = 0; i < 4; i++) {
				for (uint8_t j = 0; j < 4; j++) {
					_elems[i][j] = m._elems[i][j];
				}
			}
		}
        Matrix44(const T elems[16])
		{
			T* base = &_elems[0][0];
			for (uint8_t i = 0; i < 16; i++) {
				*(base + i) = elems[i];
			}
		}

		const T* operator [](uint8_t index) const { return _elems[index]; }
		T* operator [](uint8_t index) { return _elems[index]; }

		void operator =(const Matrix44& m)
		{
			for (uint8_t i = 0; i < 4; i++) {
				for (uint8_t j = 0; j < 4; j++) {
					_elems[i][j] = m._elems[i][j];
				}
			}
		}
		Matrix44 operator -() const
		{
			for (uint8_t i = 0; i < 4; i++) {
				for (uint8_t j = 0; j < 4; j++) {
					_elems[i][j] *= -1;
				}
			}
		}
		Matrix44 operator +(const Matrix44& m) const
		{
			Matrix44 r;
			for (uint8_t i = 0; i < 4; i++) {
				for (uint8_t j = 0; j < 4; j++) {
					r[i][j] = _elems[i][j] + m._elems[i][j];
				}
			}
			return r;
		}
		Matrix44 operator -(const Matrix44& m) const
		{
			Matrix44 r;
			for (uint8_t i = 0; i < 4; i++) {
				for (uint8_t j = 0; j < 4; j++) {
					r[i][j] = _elems[i][j] - m._elems[i][j];
				}
			}
			return r;
		}
		Matrix44 operator *(T s) const
		{
			Matrix44 r;
			for (uint8_t i = 0; i < 4; i++) {
				for (uint8_t j = 0; j < 4; j++) {
					r[i][j] = _elems[i][j] * s;
				}
			}
			return r;
		}
		Matrix44 operator *(const Matrix44& m) const
		{
			Matrix44 r;
			const T* __restrict a = &_elems[0][0];
			const T* __restrict b = &m._elems[0][0];
			T * __restrict c = &r._elems[0][0];

			T a0, a1, a2, a3;

			a0 = a[0];
			a1 = a[4];
			a2 = a[8];
			a3 = a[12];
			c[ 0] = a0 * b[ 0] + a1 * b[ 1] + a2 * b[ 2] + a3 * b[ 3];
			c[ 4] = a0 * b[ 4] + a1 * b[ 5] + a2 * b[ 6] + a3 * b[ 7];
			c[ 8] = a0 * b[ 8] + a1 * b[ 9] + a2 * b[10] + a3 * b[11];
			c[12] = a0 * b[12] + a1 * b[13] + a2 * b[14] + a3 * b[15];

			a0 = a[1];
			a1 = a[5];
			a2 = a[9];
			a3 = a[13];
			c[ 1] = a0 * b[ 0] + a1 * b[ 1] + a2 * b[ 2] + a3 * b[ 3];
			c[ 5] = a0 * b[ 4] + a1 * b[ 5] + a2 * b[ 6] + a3 * b[ 7];
			c[ 9] = a0 * b[ 8] + a1 * b[ 9] + a2 * b[10] + a3 * b[11];
			c[13] = a0 * b[12] + a1 * b[13] + a2 * b[14] + a3 * b[15];

			a0 = a[2];
			a1 = a[6];
			a2 = a[10];
			a3 = a[14];
			c[ 2] = a0 * b[ 0] + a1 * b[ 1] + a2 * b[ 2] + a3 * b[ 3];
			c[ 6] = a0 * b[ 4] + a1 * b[ 5] + a2 * b[ 6] + a3 * b[ 7];
			c[10] = a0 * b[ 8] + a1 * b[ 9] + a2 * b[10] + a3 * b[11];
			c[14] = a0 * b[12] + a1 * b[13] + a2 * b[14] + a3 * b[15];

			a0 = a[3];
			a1 = a[7];
			a2 = a[11];
			a3 = a[15];
			c[ 3] = a0 * b[ 0] + a1 * b[ 1] + a2 * b[ 2] + a3 * b[ 3];
			c[ 7] = a0 * b[ 4] + a1 * b[ 5] + a2 * b[ 6] + a3 * b[ 7];
			c[11] = a0 * b[ 8] + a1 * b[ 9] + a2 * b[10] + a3 * b[11];
			c[15] = a0 * b[12] + a1 * b[13] + a2 * b[14] + a3 * b[15];

			return r;
		}
		bool operator ==(const Matrix44& m) const
		{
			for (uint8_t i = 0; i < 4; i++) {
				for (uint8_t j = 0; j < 4; j++) {
					if (_elems[i][j] != m._elems[i][j]) {
						return false;
					}
				}
			}
			return true;
		}
		bool operator !=(const Matrix44& m) const
		{
			return !((*this) == m);
		}
		Matrix44 transposed() const
		{
			T temp[] = { _elems[0][0], _elems[1][0], _elems[2][0], _elems[3][0],
						 _elems[0][1], _elems[1][1], _elems[2][1], _elems[3][1],
						 _elems[0][2], _elems[1][2], _elems[2][2], _elems[3][2],
						 _elems[0][3], _elems[1][3], _elems[2][3], _elems[3][3] };
			return Matrix44(temp);
		}
		void transpose()
		{
			T temp[] = { _elems[0][0], _elems[1][0], _elems[2][0], _elems[3][0],
						 _elems[0][1], _elems[1][1], _elems[2][1], _elems[3][1],
						 _elems[0][2], _elems[1][2], _elems[2][2], _elems[3][2],
						 _elems[0][3], _elems[1][3], _elems[2][3], _elems[3][3] };
			*this = Matrix44(temp);
		}
		Matrix44 Inversed() const
		{
			Matrix44 orig = Matrix44(*this);
			Matrix44 iden((T)1);
			for (unsigned col = 0; col < 4; col++) {
				if (orig[col][col] == 0) {
					size_t big = col;
					for (unsigned row = 0; row < 4; row++) {
						if (fabs(orig[col][row]) > fabs(orig[col][big])) {
							big = row;
						}
					}
					// Print this is a singular matrix, return identity ?
					if (big == col) {
						std::cerr << "Singular matrix\n";
						return *this;
					}
					// Swap rows
					else for (unsigned c = 0; c < 4; c++) {
						std::swap(orig._elems[c][col], orig._elems[c][big]);
						std::swap(iden._elems[c][col], iden._elems[c][big]);
					}
				}

				// Set each row in the column to 0
				for (unsigned row = 0; row < 4; row++) {
					if (row != col) {
						T coeff = orig[col][row] / orig[col][col];
						if (coeff != 0) {
							for (unsigned c = 0; c < 4; c++) {
								orig._elems[c][row] -= coeff * orig._elems[c][col];
								iden._elems[c][row] -= coeff * iden._elems[c][col];
							}
							// Set the element to 0 for safety
							orig._elems[col][row] = 0;
						}
					}
				}
			}
			// Set each element of the diagonal to 1
			for (unsigned row = 0; row < 4; row++) {
				for (unsigned col = 0; col < 4; col++) {
					iden._elems[col][row] /= orig._elems[row][row];
				}
			}
			return iden;
		}

		static const Matrix44 identity;

		template<typename T>
		friend Vec4<T> MultiplyPoint(const Matrix44<T>& m, const Vec3<T>& v);

		std::vector<Vec4<T>> operator *(const std::vector<Vec3<T>>& vertices) const
		{
			std::vector<Vec4<T>> r;
			Vec4<T> temp;
			for (const Vec3<T>& v : vertices) {
				temp = MultiplyPoint((*this), v);
				r.push_back(temp);
			}
			return r;
		}

		template<typename T>
		friend Vec3<T> MultiplyVector(const Matrix44<T>& m, const Vec3<T>& v);

		template<typename T>
		friend ostream& operator <<(ostream& os, const Matrix44<T>& m);
    };

	template<typename T>
	Vec4<T> MultiplyPoint(const Matrix44<T>& m, const Vec3<T>& v)
	{
		Vec4<T> r;

		r[0] = m._elems[0][0] * v[0] + m._elems[1][0] * v[1] + m._elems[2][0] * v[2] + m._elems[3][0];
		r[1] = m._elems[0][1] * v[0] + m._elems[1][1] * v[1] + m._elems[2][1] * v[2] + m._elems[3][1];
		r[2] = m._elems[0][2] * v[0] + m._elems[1][2] * v[1] + m._elems[2][2] * v[2] + m._elems[3][2];
		r[3] = m._elems[0][3] * v[0] + m._elems[1][3] * v[1] + m._elems[2][3] * v[2] + m._elems[3][3];

		return r;
	}

	template<typename T>
	Vec3<T> MultiplyVector(const Matrix44<T>& m, const Vec3<T>& v)
	{
		Vec3<T> r;

		r[0] = m._elems[0][0] * v[0] + m._elems[1][0] * v[1] + m._elems[2][0] * v[2];
		r[1] = m._elems[0][1] * v[0] + m._elems[1][1] * v[1] + m._elems[2][1] * v[2];
		r[2] = m._elems[0][2] * v[0] + m._elems[1][2] * v[1] + m._elems[2][2] * v[2];

		return r;
	}

	template<typename T>
	ostream& operator <<(ostream& os, const Matrix44<T>& m)
	{
		os << '(' << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ", " << m[0][3] << ')' << endl;
		os << '(' << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ", " << m[1][3] << ')' << endl;
		os << '(' << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ", " << m[2][3] << ')' << endl;
		os << '(' << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << ", " << m[3][3] << ')' << endl;
		return os;
	}

	typedef Matrix44<float> Matrix44f;

	template<typename T>
	const Matrix44<T> Matrix44<T>::identity = Matrix44((T)1);
}}

#endif // MATRIX_H_INCLUDED
