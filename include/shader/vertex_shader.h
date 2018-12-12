#ifndef VERTEX_SHADER_H_INCLUDED
#define VERTEX_SHADER_H_INCLUDED

#include "geometry/vec2.hpp"
#include "geometry/vec3.hpp"
#include "geometry/vec4.hpp"
#include "transform/matrix.hpp"

namespace SoftwareRenderer {
namespace Shader {

	using Geometry::Vec2f;
	using Geometry::Vec3f;
	using Geometry::Vec4f;
	using Transform::Matrix44f;

	class VertexShaderPC
	{
	public:
		void Main(const Vec3f& pos, const Vec4f& color, Vec4f& outPos, Vec4f& outColor)
		{
			Vert(pos, color, outPos, outColor);
		}

		const Matrix44f* model;
		const Matrix44f* view;
		const Matrix44f* projection;
	private:
		virtual void Vert(const Vec3f& pos, const Vec4f& color, Vec4f& outPos, Vec4f& outColor) = 0;
	};

	class VertexShaderPU
	{
	public:
		void Main(const Vec3f& pos, const Vec2f& uv, Vec4f& outPos, Vec2f& outUV)
		{
			Vert(pos, uv, outPos, outUV);
		}

		const Matrix44f* model;
		const Matrix44f* view;
		const Matrix44f* projection;
	private:
		virtual void Vert(const Vec3f& pos, const Vec2f& uv, Vec4f& outPos, Vec2f& outUV) = 0;
	};

	class VertexShaderPUN
	{
	public:
		void Main(const Vec3f& pos, const Vec2f& uv, const Vec3f& normal, Vec4f& outPos)
		{
			Vert(pos, uv, normal, outPos);
		}

		const Matrix44f* model;
		const Matrix44f* view;
		const Matrix44f* modelview;
		const Matrix44f* projection;
	private:
		virtual void Vert(const Vec3f& pos, const Vec2f& uv, const Vec3f& normal, Vec4f& outPos) = 0;
	};
}}

#endif // VERTEX_SHADER_H_INCLUDED