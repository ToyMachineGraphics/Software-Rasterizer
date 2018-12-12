#ifndef BASE_COLOR_VERT_H_INCLUDED
#define BASE_COLOR_VERT_H_INCLUDED

#include "shader/vertex_shader.h"

namespace SoftwareRenderer {
namespace Shader {

	class BaseColorVert : public VertexShaderPC
	{
	private:
		virtual void Vert(const Vec3f& pos, const Vec4f& color, Vec4f& outPos, Vec4f& outColor)
		{
			outPos = Transform::MultiplyPoint<float>((*projection) * (*view) * (*model), pos);
			outColor = color;
		}
	};
}}

#endif // BASE_COLOR_VERT_H_INCLUDED