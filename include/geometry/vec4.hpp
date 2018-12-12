#ifndef VECTOR4_H_INCLUDED
#define VECTOR4_H_INCLUDED

#include <cmath>
#include <iostream>

namespace SoftwareRenderer {
namespace Geometry {

	using std::cout;
	using std::ostream;

	template<typename T>
    class Vec4
    {
    public:
        Vec4() : x(0), y(0), z(0), w(0) {}
		Vec4(T identity) : x(identity), y(identity), z(identity), w(identity) {}
        Vec4(T initX, T initY, T initZ, T initW) : x(initX), y(initY), z(initZ), w(initW) {}

		void operator =(const Vec4& v) { x = v.x, y = v.y, z = v.z, w = v.w; }
        Vec4 operator -() const { return Vec4(-x, -y, -z, -w); }
        Vec4 operator +(const Vec4& v) { return Vec4(x + v.x, y + v.y, z + v.z, w + v.w); }
        Vec4 operator -(const Vec4& v) const { return Vec4(x - v.x, y - v.y, z - v.z, w - v.w); }
        Vec4 operator *(T s) const { return Vec4(this->x * s, this->y * s, this->z * s, this->w * s); }
		Vec4 operator /(T s) const { return Vec4(this->x / s, this->y / s, this->z / s, this->w / s); }
		bool operator ==(const Vec4& v) const { return x == v.x && y == v.y && z == v.z && w == v.w; }
        bool operator !=(const Vec4& v) const { return !(*this == v); }
		Vec4& operator *= (const T &s)
		{
			x *= s, y *= s, z *= s, w *= s;
			return *this;
		}
		Vec4& operator /= (const T &s)
		{
			x /= s, y /= s, z /= s, w /= s;
			return *this;
		}
		const T& operator [] (uint8_t i) const { return (&x)[i]; }
		T& operator [] (uint8_t i) { return (&x)[i]; }

        T Dot(const Vec4& v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }
        T Length() const { return std::sqrt(Dot(*this, *this)); }

		friend Vec4 operator *(const T& s, const Vec4& v)
		{
			return Vec4<T>(s * v.x, s * v.y, s * v.z, s * v.w);
		}
		friend ostream& operator <<(ostream& os, const Vec4<T>& v)
		{
			os << '(' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ')';
			return os;
		}

		T x, y, z, w;
		const static Vec4 zero;
    };

	typedef Vec4<float> Vec4f;
}}

#endif // VECTOR4_H_INCLUDED
