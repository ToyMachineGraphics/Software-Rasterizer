/*#include "../../include/lib/SDL_wrapper.h"
#include "../../include/context.h"
#include "../../include/rasterize/rasterize.h"
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cmath>

namespace SoftwareRenderer {
namespace Rasterize
{
    using std::min;
    using std::max;
    using std::min_element;
    using std::max_element;
    using std::fill;
    using std::round;
    using SoftwareRenderer::Context;

#ifdef DEBUG
    void Polygon(vector<Vector3> vertices)
    {
        float minX, maxX, minY, maxY;
        minX = maxX = vertices[0].x;
        minY = maxY = vertices[0].y;
        for (unsigned int i = 0; i < vertices.size(); i++) {
            if (vertices[i].x < minX) {
                minX = vertices[i].x;
            }
            if (vertices[i].x > maxX) {
                maxX = vertices[i].x;
            }
            if (vertices[i].y < minY) {
                minY = vertices[i].y;
            }
            if (vertices[i].y > maxY) {
                maxY = vertices[i].y;
            }
        }

        vector<float> dXs, dYs;
        vector<float> efs(vertices.size());
        for (unsigned int i = 0; i < vertices.size(); i++) {
            dXs.push_back(vertices[(i + 1) % vertices.size()].x - vertices[i].x);
            dYs.push_back(vertices[(i + 1) % vertices.size()].y - vertices[i].y);
        }
        fill(efs.begin(), efs.end(), 0);

        for (int row = (int)minY; row <= (int)maxY; row++) {
            for (int col = (int)minX; col <= (int)maxX; col++) {
                for (unsigned int i = 0; i < vertices.size(); i++) {
                    efs[i] = (row - vertices[i].y) * dXs[i] - (col - vertices[i].x) * dYs[i];
                }
                bool allOK = true;
                for (unsigned int i = 0; i < vertices.size(); i++) {
                    allOK = allOK && (efs[i] >= 0);
                }
                if (allOK) {
                    DrawPixel(col, row);
                }
            }
        }
    }
#endif // DEBUG

	template <typename T>
	void Triangle(const VertexPC<T>& v0, const VertexPC<T>& v1, const VertexPC<T>& v2)
	{
		int minX, maxX, minY, maxY;
		minX = round(min({ v0.pos.x, v1.pos.x, v2.pos.x }));
		maxX = round(max({ v0.pos.x, v1.pos.x, v2.pos.x }));
		minY = round(min({ v0.pos.y, v1.pos.y, v2.pos.y }));
		maxY = round(max({ v0.pos.y, v1.pos.y, v2.pos.y }));
		minX = max(0, min(minX, Context::GetInstance().GetDimension().first - 1));
		maxX = max(0, min(maxX, Context::GetInstance().GetDimension().first - 1));
		minY = max(0, min(minY, Context::GetInstance().GetDimension().second - 1));
		maxY = max(0, min(maxY, Context::GetInstance().GetDimension().second - 1));
		if ((v0.pos.x == v1.pos.x && v1.pos.x == v2.pos.x) || (v0.pos.y == v1.pos.y && v1.pos.y == v2.pos.y)) {
			return;
		}

		T ef01 = EdgeFunc(v2.pos.x, v2.pos.y, v0.pos.x, v0.pos.y, v1.pos.x - v0.pos.x, v1.pos.y - v0.pos.y);
		T ef12 = EdgeFunc(v0.pos.x, v0.pos.y, v1.pos.x, v1.pos.y, v2.pos.x - v1.pos.x, v2.pos.y - v1.pos.y);
		T ef20 = EdgeFunc(v1.pos.x, v1.pos.y, v2.pos.x, v2.pos.y, v0.pos.x - v2.pos.x, v0.pos.y - v2.pos.y);
		for (int row = minY; row <= maxY; row++) {
			for (int col = minX; col <= maxX; col++) {
				T subArea01 = ef01 - (col - v2.pos.x) * (v1.pos.y - v0.pos.y) + (row - v2.pos.y) * (v1.pos.x - v0.pos.x);
				T subArea12 = ef12 - (col - v0.pos.x) * (v2.pos.y - v1.pos.y) + (row - v0.pos.y) * (v2.pos.x - v1.pos.x);
				T subArea20 = ef20 - (col - v1.pos.x) * (v0.pos.y - v2.pos.y) + (row - v1.pos.y) * (v0.pos.x - v2.pos.y);
				if (subArea01 >= 0 && subArea12 >= 0 && subArea20 >= 0) {
					T areaRatio01 = subArea01 / ef01;
					T areaRatio12 = subArea12 / ef12;
					T areaRatio20 = subArea20 / ef01;

					Vec4<T> c0(v0.color.x, v0.color.y, v0.color.z, v0.color.w);
					Vec4<T> c1(v1.color.x, v1.color.y, v1.color.z, v1.color.w);
					Vec4<T> c2(v2.color.x, v2.color.y, v2.color.z, v2.color.w);
					//array<Vector2i, 3> uvsi{ uv0, uv1, uv2 };
					float w = areaRatio01 / v2.color.w + areaRatio12 / v0.color.w + areaRatio20 / v1.color.w;
					if (w == 0) {
						continue;
					}

					Vec4<T> color = ((v0.color * areaRatio12 / w0 + v1.color * areaRatio20 / w1 + v2.color * areaRatio01 / w2) / w);
					SetColor(color.x, color.y, color.z, color.w);
					DrawPixel(col, row);
				}
			}
		}
	}

    void TriangleColor(const Vec3f& v0, const Vec3f& v1, const Vec3f& v2, array<Vector4, 3> colors)
    {
        int minX, maxX, minY, maxY;
        minX = round(min({ vertices[0].x, vertices[1].x, vertices[2].x })) * SUBPIXEL;
        maxX = round(max({ vertices[0].x, vertices[1].x, vertices[2].x })) * SUBPIXEL;
        minY = round(min({ vertices[0].y, vertices[1].y, vertices[2].y })) * SUBPIXEL;
        maxY = round(max({ vertices[0].y, vertices[1].y, vertices[2].y })) * SUBPIXEL;

        int x0 = round(vertices[0].x * SUBPIXEL);
        int x1 = round(vertices[1].x * SUBPIXEL);
        int x2 = round(vertices[2].x * SUBPIXEL);
        int y0 = round(vertices[0].y * SUBPIXEL);
        int y1 = round(vertices[1].y * SUBPIXEL);
        int y2 = round(vertices[2].y * SUBPIXEL);

        int64_t ef01 = EdgeFunc(x2, y2, x0, y0, x1 - x0, y1 - y0);
        int64_t ef12 = EdgeFunc(x0, y0, x1, y1, x2 - x1, y2 - y1);
        int64_t ef20 = EdgeFunc(x1, y1, x2, y2, x0 - x2, y0 - y2);

        for (int row = minY; row <= maxY; row+=SUBPIXEL) {
            for (int col = minX; col <= maxX; col+=SUBPIXEL) {
                int64_t subArea01 = ef01 - (int64_t)(col - x2) * (y1 - y0) / SUBPIXEL + (int64_t)(row - y2) * (x1 - x0) / SUBPIXEL;
                int64_t subArea12 = ef12 - (int64_t)(col - x0) * (y2 - y1) / SUBPIXEL + (int64_t)(row - y0) * (x2 - x1) / SUBPIXEL;
                int64_t subArea20 = ef20 - (int64_t)(col - x1) * (y0 - y2) / SUBPIXEL + (int64_t)(row - y1) * (x0 - x2) / SUBPIXEL;
                if (subArea01 >= 0 && subArea12 >= 0 && subArea20 >= 0) {
                    int areaRatio01 = subArea01 * SUBCOLOR / ef01;
                    int areaRatio12 = subArea12 * SUBCOLOR / ef12;
                    int areaRatio20 = subArea20 * SUBCOLOR / ef01;

                    Vector4i color0(colors[0].x, colors[0].y, colors[0].z, colors[0].w);
                    Vector4i color1(colors[1].x, colors[1].y, colors[1].z, colors[1].w);
                    Vector4i color2(colors[2].x, colors[2].y, colors[2].z, colors[2].w);
                    Vector4i color(color0 * areaRatio12 / SUBCOLOR + color1 * areaRatio20 / SUBCOLOR + color2 * areaRatio01 / SUBCOLOR);
                    SetColor(color.x, color.y, color.z, color.w);
                    DrawPixel(col / 1.0f / SUBPIXEL, row / 1.0f / SUBPIXEL);
                }
            }
        }
    }

    void TriangleTexture(array<Vector3, 3> vertices, array<Vector2, 3> uvs, IImage& img)
    {
        int minX, maxX, minY, maxY;
        minX = round(min({ vertices[0].x, vertices[1].x, vertices[2].x })) * SUBPIXEL;
        maxX = round(max({ vertices[0].x, vertices[1].x, vertices[2].x })) * SUBPIXEL;
        minY = round(min({ vertices[0].y, vertices[1].y, vertices[2].y })) * SUBPIXEL;
        maxY = round(max({ vertices[0].y, vertices[1].y, vertices[2].y })) * SUBPIXEL;
        if (minX < 0) {
            minX = 0;
        }
        if (maxX >= Context::GetInstance().GetDimension().first) {
            maxX = (Context::GetInstance().GetDimension().first - 1) * SUBPIXEL;
        }
        if (minY < 0) {
            minY = 0;
        }
        if (maxY >= Context::GetInstance().GetDimension().second) {
            maxY = (Context::GetInstance().GetDimension().second - 1) * SUBPIXEL;
        }

        int x0 = round(vertices[0].x * SUBPIXEL);
        int x1 = round(vertices[1].x * SUBPIXEL);
        int x2 = round(vertices[2].x * SUBPIXEL);
        int y0 = round(vertices[0].y * SUBPIXEL);
        int y1 = round(vertices[1].y * SUBPIXEL);
        int y2 = round(vertices[2].y * SUBPIXEL);
        int z0 = round(vertices[0].z * SUBUV);
        int z1 = round(vertices[1].z * SUBUV);
        int z2 = round(vertices[2].z * SUBUV);

        if ((x0 == x1 && x1 == x2) || (y0 == y1 && y1 == y2)) {
            return;
        }

        int64_t ef01 = EdgeFunc(x2, y2, x0, y0, x1 - x0, y1 - y0);
        int64_t ef12 = EdgeFunc(x0, y0, x1, y1, x2 - x1, y2 - y1);
        int64_t ef20 = EdgeFunc(x1, y1, x2, y2, x0 - x2, y0 - y2);

        for (int row = minY; row <= maxY; row+=SUBPIXEL) {
            for (int col = minX; col <= maxX; col+=SUBPIXEL) {
                int64_t subArea01 = ef01 - (int64_t)(col - x2) * (y1 - y0) / SUBPIXEL + (int64_t)(row - y2) * (x1 - x0) / SUBPIXEL;
                int64_t subArea12 = ef12 - (int64_t)(col - x0) * (y2 - y1) / SUBPIXEL + (int64_t)(row - y0) * (x2 - x1) / SUBPIXEL;
                int64_t subArea20 = ef20 - (int64_t)(col - x1) * (y0 - y2) / SUBPIXEL + (int64_t)(row - y1) * (x0 - x2) / SUBPIXEL;
                if (subArea01 >= 0 && subArea12 >= 0 && subArea20 >= 0) {
                    int areaRatio01 = subArea01 * SUBUV / ef01;
                    int areaRatio12 = subArea12 * SUBUV / ef12;
                    int areaRatio20 = subArea20 * SUBUV / ef01;

                    Vector2i uv0{(int)(uvs[0].x * SUBUV), (int)(uvs[0].y * SUBUV)};
                    Vector2i uv1{(int)(uvs[1].x * SUBUV), (int)(uvs[1].y * SUBUV)};
                    Vector2i uv2{(int)(uvs[2].x * SUBUV), (int)(uvs[2].y * SUBUV)};
                    array<Vector2i, 3> uvsi{ uv0, uv1, uv2 };
                    int w = areaRatio01 * SUBUV / z2 + areaRatio12 * SUBUV / z0 + areaRatio20 * SUBUV / z1;
                    if (w == 0) {
                        continue;
                    }
                    Vector2i uv = ((uvsi[0] * areaRatio12 / z0 + uvsi[1] * areaRatio20 / z1 + uvsi[2] * areaRatio01 / z2) * SUBUV / w);
                    //Vector2i uv = ((uvsi[0] * areaRatio12 + uvsi[1] * areaRatio20 + uvsi[2] * areaRatio01) / SUBUV);
                    float u = uv.x / 1.0f / SUBUV;
                    float v = uv.y / 1.0f / SUBUV;
                    uint8_t r, g, b, a;
                    if (u > 1 || v > 1 || u < 0 || v < 0) {
                        r = 0;
                        g = 255;
                        b = 128;
                        a = 255;
                    } else {
                        img.GetColor(v * 255, u * 255, r, g, b, a);
                    }
                    SetColor(r, g, b, a);
                    //SetColor(0, 255, 127, 0);
                    DrawPixel(col / SUBPIXEL, row / SUBPIXEL);
                }
            }
        }
    }

    void TriangleTexture(vector<Vector4> vertices, vector<Vector2> uvs,  IImage& img)
    {
        int minX, maxX, minY, maxY;
        minX = round(min({ vertices[0].x, vertices[1].x, vertices[2].x })) * SUBPIXEL;
        maxX = round(max({ vertices[0].x, vertices[1].x, vertices[2].x })) * SUBPIXEL;
        minY = round(min({ vertices[0].y, vertices[1].y, vertices[2].y })) * SUBPIXEL;
        maxY = round(max({ vertices[0].y, vertices[1].y, vertices[2].y })) * SUBPIXEL;
        if (minX < 0) {
            minX = 0;
        }
        if (maxX >= Context::GetInstance().GetDimension().first) {
            maxX = (Context::GetInstance().GetDimension().first - 1) * SUBPIXEL;
        }
        if (minY < 0) {
            minY = 0;
        }
        if (maxY >= Context::GetInstance().GetDimension().second) {
            maxY = (Context::GetInstance().GetDimension().second - 1) * SUBPIXEL;
        }

        int x0 = round(vertices[0].x * SUBPIXEL);
        int x1 = round(vertices[1].x * SUBPIXEL);
        int x2 = round(vertices[2].x * SUBPIXEL);
        int y0 = round(vertices[0].y * SUBPIXEL);
        int y1 = round(vertices[1].y * SUBPIXEL);
        int y2 = round(vertices[2].y * SUBPIXEL);
        int w0 = round(vertices[0].w * SUBUV);
        int w1 = round(vertices[1].w * SUBUV);
        int w2 = round(vertices[2].w * SUBUV);

        if ((x0 == x1 && x1 == x2) || (y0 == y1 && y1 == y2)) {
            return;
        }

        if (vertices[0].w < 0 || vertices[1].w < 0 || vertices[2].w < 0) {
            std::cout << "negative w\n";
        }

        int64_t ef01 = EdgeFunc(x2, y2, x0, y0, x1 - x0, y1 - y0);
        int64_t ef12 = EdgeFunc(x0, y0, x1, y1, x2 - x1, y2 - y1);
        int64_t ef20 = EdgeFunc(x1, y1, x2, y2, x0 - x2, y0 - y2);

        for (int row = minY; row <= maxY; row+=SUBPIXEL) {
            for (int col = minX; col <= maxX; col+=SUBPIXEL) {
                int64_t subArea01 = ef01 - (int64_t)(col - x2) * (y1 - y0) / SUBPIXEL + (int64_t)(row - y2) * (x1 - x0) / SUBPIXEL;
                int64_t subArea12 = ef12 - (int64_t)(col - x0) * (y2 - y1) / SUBPIXEL + (int64_t)(row - y0) * (x2 - x1) / SUBPIXEL;
                int64_t subArea20 = ef20 - (int64_t)(col - x1) * (y0 - y2) / SUBPIXEL + (int64_t)(row - y1) * (x0 - x2) / SUBPIXEL;
                if (subArea01 >= 0 && subArea12 >= 0 && subArea20 >= 0) {
                    int areaRatio01 = subArea01 * SUBUV / ef01;
                    int areaRatio12 = subArea12 * SUBUV / ef12;
                    int areaRatio20 = subArea20 * SUBUV / ef01;

                    Vector2i uv0{(int)(uvs[0].x * SUBUV), (int)(uvs[0].y * SUBUV)};
                    Vector2i uv1{(int)(uvs[1].x * SUBUV), (int)(uvs[1].y * SUBUV)};
                    Vector2i uv2{(int)(uvs[2].x * SUBUV), (int)(uvs[2].y * SUBUV)};
                    array<Vector2i, 3> uvsi{ uv0, uv1, uv2 };
                    int w = areaRatio01 * SUBUV / w2 + areaRatio12 * SUBUV / w0 + areaRatio20 * SUBUV / w1;
                    if (w == 0) {
                        continue;
                    }
                    Vector2i uv = ((uvsi[0] * areaRatio12 / w0 + uvsi[1] * areaRatio20 / w1 + uvsi[2] * areaRatio01 / w2) * SUBUV / w);
                    //Vector2i uv = ((uvsi[0] * areaRatio12 + uvsi[1] * areaRatio20 + uvsi[2] * areaRatio01) / SUBUV);
                    float u = uv.x / 1.0f / SUBUV;
                    float v = uv.y / 1.0f / SUBUV;
                    uint8_t r, g, b, a;
                    if (u > 1 || v > 1 || u < 0 || v < 0) {
                        r = 0;
                        g = 255;
                        b = 128;
                        a = 255;
                    } else {
                        img.GetColor(v * 255, u * 255, r, g, b, a);
                        r = 0;
                        g = 255;
                        b = 128;
                        a = 255;
                    }
                    SetColor(r, g, b, a);
                    //SetColor(0, 255, 127, 0);
                    DrawPixel(col / SUBPIXEL, row / SUBPIXEL);
                }
            }
        }
    }
}}*/
