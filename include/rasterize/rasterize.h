#ifndef RASTERIZE_H_INCLUDED
#define RASTERIZE_H_INCLUDED

#pragma once

#include "geometry/vec2.hpp"
#include "geometry/vec3.hpp"
#include "geometry/vec4.hpp"
#include "lib/SDL_wrapper.h"
#include "context.h"
#include "texture.h"
#include "shader/base_color_frag.h"
#include "shader/base_uv_frag.h"
#include "shader/phong_shading_frag.h"

#include <iostream>
#include <algorithm>

namespace SoftwareRenderer {
namespace Rasterize {

	using Geometry::Vec2;
	using Geometry::Vec3;
	using Geometry::Vec4;
	using SoftwareRenderer::Context;

	using std::min;
	using std::max;
	using std::round;

	template <typename T>
	struct VertexPC
	{
		Vec4<T> pos;
		Vec4<T> color;
	};

	template <typename T>
	struct VertexPU
	{
		Vec4<T> pos;
		Vec2<T> uv;
	};

	template <typename T>
	struct LightingVertex
	{
		Vec4<T> pos;
		Vec2<T> uv;
		Vec3<T> normal;
		Vec3<T> worldPos;
	};

	template <typename T>
	T EdgeFunc(T x, T y, T X, T Y, T dX, T dY)
	{
		return (y - Y) * dX - (x - X) * dY;
	}

	template <typename T>
	bool CollinearCheck(const Vec4<T>& pos0, const Vec4<T>& pos1, const Vec4<T>& pos2, int& minX, int& maxX, int& minY, int& maxY)
	{
		minX = (int)round(min({ pos0.x, pos1.x, pos2.x }));
		maxX = (int)round(max({ pos0.x, pos1.x, pos2.x }));
		minY = (int)round(min({ pos0.y, pos1.y, pos2.y }));
		maxY = (int)round(max({ pos0.y, pos1.y, pos2.y }));
		if ((pos0.x == pos1.x && pos1.x == pos2.x) || (pos0.y == pos1.y && pos1.y == pos2.y)) {
			return true;
		}
		return false;
	}

#define RENDER_LOOP_BEGIN() \
	int minX, maxX, minY, maxY; \
	if (CollinearCheck(v0.pos, v1.pos, v2.pos, minX, maxX, minY, maxY)) { \
		return; \
	} \
 \
	T ef01 = EdgeFunc(v2.pos.x, v2.pos.y, v0.pos.x, v0.pos.y, v1.pos.x - v0.pos.x, v1.pos.y - v0.pos.y); \
	T ef12 = EdgeFunc(v0.pos.x, v0.pos.y, v1.pos.x, v1.pos.y, v2.pos.x - v1.pos.x, v2.pos.y - v1.pos.y); \
	T ef20 = EdgeFunc(v1.pos.x, v1.pos.y, v2.pos.x, v2.pos.y, v0.pos.x - v2.pos.x, v0.pos.y - v2.pos.y); \
	for (int row = minY; row <= maxY; row++) { \
		for (int col = minX; col <= maxX; col++) { \
			if (row < 0 || row >= context.Dimension().second || \
				col < 0 || col >= context.Dimension().first) { \
				continue; \
			} \
			T subArea01 = ef01 - (col - v2.pos.x) * (v1.pos.y - v0.pos.y) + (row - v2.pos.y) * (v1.pos.x - v0.pos.x); \
			T subArea12 = ef12 - (col - v0.pos.x) * (v2.pos.y - v1.pos.y) + (row - v0.pos.y) * (v2.pos.x - v1.pos.x); \
			T subArea20 = ef20 - (col - v1.pos.x) * (v0.pos.y - v2.pos.y) + (row - v1.pos.y) * (v0.pos.x - v2.pos.x); \
			bool pass = true; \
			if (context.cullFaceMode == Context::CullFaceMode::BACK) { \
				pass = subArea01 >= 0 && subArea12 >= 0 && subArea20 >= 0; \
			} \
			else if (context.cullFaceMode == Context::CullFaceMode::FRONT) { \
				pass = subArea01 <= 0 && subArea12 <= 0 && subArea20 <= 0; \
			} \
			else { \
				pass = (subArea01 >= 0 && subArea12 >= 0 && subArea20 >= 0) || (subArea01 <= 0 && subArea12 <= 0 && subArea20 <= 0); \
			} \
			if (pass) { \
				T areaRatio01 = subArea01 / ef01; \
				T areaRatio12 = subArea12 / ef12; \
				T areaRatio20 = subArea20 / ef01; \
\
				T wInv = 1.0 / (areaRatio12 / v0.pos.w + areaRatio20 / v1.pos.w + areaRatio01 / v2.pos.w); \
				if (context.depthTest) { \
					T z = (v0.pos.z * areaRatio12 / v0.pos.w + v1.pos.z * areaRatio20 / v1.pos.w + v2.pos.z * areaRatio01 / v2.pos.w) * wInv; \
					float n, f; \
					context.DepthRange(n, f); \
					if (n < f && z >(T)context.DepthValueAt(col, row) || \
						n > f && z < (T)context.DepthValueAt(col, row)) \
					{ \
						continue; \
					} \
					else { \
						context.DepthValueAt(col, row) = z; \
					} \
				}

 #define RENDER_LOOP_END() \
				DrawPixel(col, row); \
			} \
		} \
	}

	template <typename T>
	void Triangle(const VertexPC<T>& v0, const VertexPC<T>& v1, const VertexPC<T>& v2, Shader::BaseColorFrag* frag = nullptr)
	{
		RENDER_LOOP_BEGIN()

			Vec4<T> color = (v0.color * areaRatio12 / v0.pos.w + v1.color * areaRatio20 / v1.pos.w + v2.color * areaRatio01 / v2.pos.w) * wInv;
			frag->Main(color, color);
			SetColor(color.x, color.y, color.z, color.w);

		RENDER_LOOP_END()
	}

	template <typename T>
	void Triangle(const VertexPU<T>& v0, const VertexPU<T>& v1, const VertexPU<T>& v2, Shader::BaseUVFrag* shader = nullptr)
	{
		RENDER_LOOP_BEGIN()

			Vec2<T> uv = (areaRatio12 * v0.uv / v0.pos.w + areaRatio20 * v1.uv / v1.pos.w + areaRatio01 * v2.uv / v2.pos.w) * wInv;
			//Vec2<T> uv = areaRatio12 * v0.uv + areaRatio20 * v1.uv + areaRatio01 * v2.uv;
			uv.x = max<T>(0.0, min<T>(uv.x, 1.0));
			uv.y = max<T>(0.0, min<T>(uv.y, 1.0));
			Vec4<uint8_t> outColor;
			shader->Main(uv, outColor);
			SetColor(outColor.x, outColor.y, outColor.z, outColor.w);

		RENDER_LOOP_END()
	}

	template <typename T>
	void Triangle(const LightingVertex<T>& v0, const LightingVertex<T>& v1, const LightingVertex<T>& v2, Shader::PhongShadingFrag* shader = nullptr)
	{
		RENDER_LOOP_BEGIN()

			Vec2<T> uv = (areaRatio12 * v0.uv / v0.pos.w + areaRatio20 * v1.uv / v1.pos.w + areaRatio01 * v2.uv / v2.pos.w) * wInv;
			//Vec2<T> uv = areaRatio12 * v0.uv + areaRatio20 * v1.uv + areaRatio01 * v2.uv;
			uv.x = max<T>(0.0, min<T>(uv.x, 1.0));
			uv.y = max<T>(0.0, min<T>(uv.y, 1.0));
			Vec3<T> normal = (areaRatio12 * v0.normal / v0.pos.w + areaRatio20 * v1.normal / v1.pos.w + areaRatio01 * v2.normal / v2.pos.w) * wInv;
			Vec3<T> worldPos = (areaRatio12 * v0.worldPos / v0.pos.w + areaRatio20 * v1.worldPos / v1.pos.w + areaRatio01 * v2.worldPos / v2.pos.w) * wInv;
			Vec4<float> outColor;
			shader->vinWorldPos = worldPos;
			shader->Main(uv, normal, outColor);
			SetColor(outColor.x, outColor.y, outColor.z, outColor.w);

		RENDER_LOOP_END()
	}

#undef RENDER_LOOP_BEGIN()
#undef RENDER_LOOP_END()
}}

#endif // RASTERIZE_H_INCLUDED
