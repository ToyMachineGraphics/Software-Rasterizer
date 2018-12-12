#ifndef BASE_UV_FRAG_H_INCLUDED
#define BASE_UV_FRAG_H_INCLUDED

#include "shader/fragment_shader.h"
#include "context.h"

namespace SoftwareRenderer {
namespace Shader {

	class BaseUVFrag : public FragmentShaderU
	{
		void Frag(const Vec2f& uv, Vec4<uint8_t>& outColor)
		{
			Vec2f outUV;
			outUV.x = (int)(uv.x * (texture->Width() - 1));
			outUV.y = (int)(uv.y * (texture->Height() - 1));
			int pi = (int)(outUV.y * texture->Width() + outUV.x) * texture->ChannelCount();
			const uint8_t* pixels = texture->Pixels();
			const uint8_t* p = &pixels[pi];
			if (texture->ChannelCount() == 4) {
				outColor.x = *p;
				outColor.y = *(p + 1);
				outColor.z = *(p + 2);
				outColor.w = *(p + 3);
			}
			else if (texture->ChannelCount() == 3) {
				outColor.x = *p;
				outColor.y = *(p + 1);
				outColor.z = *(p + 2);
				outColor.w = UINT8_MAX;
			}
		}
	};
}}

#endif