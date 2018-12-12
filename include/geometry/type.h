#ifndef GEOMETRY_TYPE_H_INCLUDED
#define GEOMETRY_TYPE_H_INCLUDED

#include "vec2.hpp"

namespace SoftwareRenderer {
namespace Geometry {

	template <typename T>
    struct Triangle2D {
        Vec2<T> p[3];
    };

	using Triangle2Df = Triangle2D<float>;
}}

#endif // GEOMETRY_TYPE_H_INCLUDED
