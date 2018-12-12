#ifndef BLINN_PHONG_SHADING_VERT_H_INCLUDED
#define BLINN_PHONG_SHADING_VERT_H_INCLUDED

#include "shader/vertex_shader.h"
#include "transform/matrix.hpp"
#include <algorithm>

namespace SoftwareRenderer {
namespace Shader {

	using namespace Transform;

	class PhongShadingVert : public VertexShaderPUN
	{
	public:
		Vec3f voutWorldPos;
		Vec3f voutNormal;
	private:
		void Vert(const Vec3f& pos, const Vec2f& uv, const Vec3f& normal, Vec4f& outPos)
		{
			Vec4f tmp = MultiplyPoint(*model, pos);
			voutWorldPos.x = tmp.x;
			voutWorldPos.y = tmp.y;
			voutWorldPos.z = tmp.z;
			Vec3f norm(normal.x, normal.y, normal.z);
			norm.Normalize();
			voutNormal = MultiplyVector<float>(model->Inversed().transposed(), norm);
			voutNormal.Normalize();
			outPos = MultiplyPoint(*projection * *view * *model, pos);
		}
	};
}}

#endif // BLINN_PHONG_SHADING_VERT_H_INCLUDED