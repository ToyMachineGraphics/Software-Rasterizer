#ifndef BASE_UV_VERT_H_INCLUDED
#define BASE_UV_VERT_H_INCLUDED

#include "shader/vertex_shader.h"

namespace SoftwareRenderer {
	namespace Shader {

		class BaseUVVert : public VertexShaderPU
		{
		private:
			virtual void Vert(const Vec3f& pos, const Vec2f& uv, Vec4f& outPos, Vec2f& outUV)
			{
				outPos = Transform::MultiplyPoint<float>((*projection) * (*view) * (*model), pos);
				outUV = uv;
			}
		};
	}
}

#endif // BASE_UV_VERT_H_INCLUDED