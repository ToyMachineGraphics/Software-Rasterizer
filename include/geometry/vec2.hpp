#ifndef VECTOR2_H_INCLUDED
#define VECTOR2_H_INCLUDED

#include <cmath>
#include <iostream>

namespace SoftwareRenderer {
namespace Geometry {

	using std::cout;
	using std::ostream;

	template<typename T>
    class Vec2
    {
    public:
		Vec2() : x(0), y(0) {}
		Vec2(T identity) : x(identity), y(identity) {}
		Vec2(T initX, T initY) : x(initX), y(initY) {}

		void operator =(const Vec2& v) { x = v.x, y = v.y; }
		Vec2 operator -() const { return Vec2(-x, -y); }
		Vec2 operator +(const Vec2& v) const { return Vec2(x + v.x, y + v.y); }
        Vec2 operator -(const Vec2& v) const { return Vec2(x - v.x, y - v.y); }
        Vec2 operator *(T s) const { return Vec2(this->x * s, this->y * s); }
		Vec2 operator /(T s) const { return Vec2(this->x / s, this->y / s); }
        bool operator ==(const Vec2& v) const { return x == v.x && y == v.y; }
        bool operator !=(const Vec2& v) const { return !(*this == v); }
		Vec2& operator *= (const T &s)
		{
			x *= s, y *= s;
			return *this;
		}
		Vec2& operator /= (const T &s)
		{
			x /= s, y /= s;
			return *this;
		}


		T Dot(const Vec2& v) const { return x * v.x + y * v.y; }
        T Length() const { return std::sqrt(Dot(*this, *this)); }
        T Determinant(const Vec2& v) const { return x * v.y - y * v.x; }

		friend Vec2 operator *(const T& s, const Vec2<T>& v)
		{
			return Vec2(s * v.x, s * v.y);
		}
		friend ostream& operator <<(ostream& os, const Vec2<T>& v)
		{
			os << '(' << v.x << ", " << v.y << ")";
			return os;
		}

		T x, y;
		const static Vec2 zero;
    };

	typedef Vec2<float> Vec2f;
	typedef Vec2<int> Vec2i;
}}

#endif // VECTOR2_H_INCLUDED
