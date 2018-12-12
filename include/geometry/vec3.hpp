#ifndef VECTOR3_H_INCLUDED
#define VECTOR3_H_INCLUDED

#include <cmath>
#include <iostream>
#include <cassert>

namespace SoftwareRenderer {
namespace Geometry {

	using std::cout;
	using std::ostream;

	template<typename T>
    class Vec3
    {
    public:
        Vec3() : x(0), y(0), z(0) {}
		Vec3(T identity) : x(identity), y(identity), z(identity) {}
        Vec3(T initX, T initY, T initZ) : x(initX), y(initY), z(initZ) {}

		void operator =(const Vec3& v) { x = v.x, y = v.y, z = v.z; }
        Vec3 operator -() const { return Vec3(-x, -y, -z); }
        Vec3 operator +(const Vec3& v) { return Vec3(x + v.x, y + v.y, z + v.z); }
        Vec3 operator -(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
        Vec3 operator *(T s) const { return Vec3(this->x * s, this->y * s, this->z * s); }
		Vec3 operator *(const Vec3& v) const { return Vec3(this->x * v.x, this->y * v.y, this->z * v.z); }
		Vec3 operator /(T s) const { return Vec3(this->x / s, this->y / s, this->z / s); }
		bool operator ==(const Vec3& v) const { return x == v.x && y == v.y && z == v.z; }
        bool operator !=(const Vec3& v) const { return !(*this == v); }
		Vec3& operator *= (const T &s)
		{
			x *= s, y *= s, z *= s;
			return *this;
		}
		Vec3& operator /= (const T &s)
		{
			x /= s, y /= s, z /= s;
			return *this;
		}
		const T& operator [] (uint8_t i) const { return (&x)[i]; }
		T& operator [] (uint8_t i) { return (&x)[i]; }

        T Dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
        Vec3 Cross(const Vec3& v) const { return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
        T Length() const { return std::sqrt(Dot(*this, *this)); }
		void Normalize()
		{
			T n = Dot(*this);
			if (n > 0) {
				T factor = 1 / std::sqrt(n);
				x *= factor, y *= factor, z *= factor;
			}
		}
		Vec3 Normalized() const
		{
			T n = Dot(*this);
			if (n > 0) {
				T factor = 1 / std::sqrt(n);
				return Vec3(x * factor, y * factor, z * factor);
			}
			cout << "(x, y, z) = (" << x << ", " << y << ", " << z << ") normalization failed.\n";
			assert(false);
		}

		friend Vec3 operator *(const T& s, const Vec3& v)
		{
			return Vec3<T>(s * v.x, s * v.y, s * v.z);
		}
		friend ostream& operator <<(ostream& os, const Vec3<T>& v)
		{
			os << '(' << v.x << ", " << v.y << ", " << v.z << ')';
			return os;
		}

		T x, y, z;
		const static Vec3 zero;
    };

	typedef Vec3<float> Vec3f;
}}

#endif // VECTOR3_H_INCLUDED
