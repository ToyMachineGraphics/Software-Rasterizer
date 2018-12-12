#ifndef BASE_COLOR_FRAG_H_INCLUDED
#define BASE_COLOR_FRAG_H_INCLUDED

#include "shader/fragment_shader.h"

namespace SoftwareRenderer {
namespace Shader {

	class BaseColorFrag : public FragmentShaderC
	{
		void Frag(const Vec4f& color, Vec4f& outColor)
		{
			outColor = color;
			outColor = Vec4f(0.21f * color.x + 0.72f * color.y + 0.07f * color.z);
		}
	};
}}

#endif