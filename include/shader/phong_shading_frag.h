#ifndef BLINN_PHONG_SHADING_FRAG_H_INCLUDED
#define BLINN_PHONG_SHADING_FRAG_H_INCLUDED

#include "shader/fragment_shader.h"
#include "transform/matrix.hpp"
#include <algorithm>

namespace SoftwareRenderer {
namespace Shader {

	using Geometry::Vec2f;
	using Geometry::Vec3f;
	using Geometry::Vec4f;
	using namespace Transform;

	class PhongShadingFrag : public FragmentShaderPUN
	{
	public:
		Vec3f vinWorldPos;

		Vec3f worldPos;
		Light light;
	private:
		void Frag(const Vec2f& uv, const Vec3f& normal, Vec4<float>& outColor)
		{
			Vec2f outUV;
			outUV.x = (int)(uv.x * (texture->Width() - 1));
			outUV.y = (int)(uv.y * (texture->Height() - 1));
			int pi = (int)(outUV.y * texture->Width() + outUV.x) * texture->ChannelCount();
			const uint8_t* pixels = texture->Pixels();
			const uint8_t* p = &pixels[pi];
			Vec4f texColor;
			if (texture->ChannelCount() == 4) {
				texColor.x = *p;
				texColor.y = *(p + 1);
				texColor.z = *(p + 2);
				texColor.w = *(p + 3);
			}
			else if (texture->ChannelCount() == 3) {
				texColor.x = *p;
				texColor.y = *(p + 1);
				texColor.z = *(p + 2);
				texColor.w = UINT8_MAX;
			}

			Vec3f ambient = light.ambientColor;

			Vec3f norm = normal.Normalized();
			Vec3f lightDir = (light.position - vinWorldPos).Normalized();
			float diff = std::max(norm.Dot(lightDir), 0.0f);
			Vec3f diffuse = diff * light.diffuseColor;

			// specular
			Vec3f viewDir = (worldPos - vinWorldPos).Normalized();
			Vec3f reflectDir = -lightDir - 2.0 * norm * norm .Dot(-lightDir);
			float spec = std::pow(std::max(viewDir.Dot(reflectDir), 0.0f), light.shininess);
			Vec3f specular = spec * light.specularColor;

			Vec3f result = (ambient + diffuse) * Vec3f(texColor.x / UINT8_MAX, texColor.y / UINT8_MAX, texColor.z / UINT8_MAX) + specular;
			outColor.x = std::max(0.0f, std::min(result.x, 1.0f));
			outColor.y = std::max(0.0f, std::min(result.y, 1.0f));
			outColor.z = std::max(0.0f, std::min(result.z, 1.0f));
			outColor.w = 1;
		}
	};
}}

#endif // BLINN_PHONG_SHADING_FRAG_H_INCLUDED