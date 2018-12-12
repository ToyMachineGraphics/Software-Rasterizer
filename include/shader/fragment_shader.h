#ifndef FRAGMENT_SHADER_H_INCLUDED
#define FRAGMENT_SHADER_H_INCLUDED

#include "geometry/vec2.hpp"
#include "geometry/vec3.hpp"
#include "geometry/vec4.hpp"
#include "texture.h"

namespace SoftwareRenderer {
namespace Shader {

	using Geometry::Vec2f;
	using Geometry::Vec3f;
	using Geometry::Vec4;
	using Geometry::Vec4f;
	using Transform::Matrix44f;

	class FragmentShaderC
	{
	public:
		void Main(const Vec4f& color, Vec4f& outColor)
		{
			Frag(color, outColor);
		}
	private:
		virtual void Frag(const Vec4f& color, Vec4f& outColor) = 0;
	};

	class FragmentShaderU
	{
	public:
		void Main(const Vec2f& uv, Vec4<uint8_t>& outColor)
		{
			Frag(uv, outColor);
		}

		const Texture* texture;
	private:
		virtual void Frag(const Vec2f& uv, Vec4<uint8_t>& outColor) = 0;
	};

	class FragmentShaderPUN
	{
	public:
		typedef struct Light
		{
			bool isPoint;
			Vec3f position;
			Vec3f ambientColor;
			Vec3f diffuseColor;
			Vec3f specularColor;
			float shininess;
		} Light;

		void Main(const Vec2f& uv, const Vec3f& normal, Vec4<float>& outColor)
		{
			Frag(uv, normal, outColor);
		}

		const Matrix44f* model;
		const Matrix44f* view;
		const Matrix44f* projection;

		const Texture* texture;
	private:
		virtual void Frag(const Vec2f& uv, const Vec3f& normal, Vec4<float>& outColor) = 0;
	};
}}

#endif // FRAGMENT_SHADER_H_INCLUDED