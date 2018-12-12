#include "app/phong_shading.h"
#include "geometry/vec2.hpp"
#include "geometry/vec3.hpp"
#include "geometry/vec4.hpp"
#include "rasterize/rasterize.h"
#include "transform/matrix.hpp"
#include "context.h"

#include <vector>

using SoftwareRenderer::Geometry::Vec2f;
using SoftwareRenderer::Geometry::Vec3f;
using SoftwareRenderer::Geometry::Vec4f;
using namespace SoftwareRenderer::Rasterize;
using namespace SoftwareRenderer::Transform;

using std::vector;

PhongShading::PhongShading()
{
	_camera.LookAt(0.25f, 1, 2, 0.25f, 0, 0, 0, 1, 0);
	_camera.Perspective(60, 1, 1.0f, 100.0f);

	Mesh::CreateSphere(_sphere);
	_sphere.LoadTexture("asset/image/checkermap.png");
	_phongShadingFrag.worldPos = Vec3f(0.25f, 1, 2);
	_phongShadingFrag.light.position = Vec3f(1, 1, 1);
	_phongShadingFrag.light.ambientColor = Vec3f(0.125f, 0.125f, 0.125f);
	_phongShadingFrag.light.diffuseColor = Vec3f(0.875f, 0.875f, 0.875f);
	_phongShadingFrag.light.specularColor = Vec3f(1, 1, 1);
	_phongShadingFrag.light.shininess = 32;
}

void PhongShading::RenderTask()
{
	context.ClearDepthBuffer();
	float n, f;
	context.DepthRange(n, f);

	_sphere.AngleAxis(Vec3f(0, 1, 0), 1.0 / 180.0 * 3.141592654);

	_phongShadingVert.model = &_sphere.ModelMatrix();
	_phongShadingVert.view = &_camera.ViewMatrix();
	_phongShadingVert.modelview = &_camera.CameraMatrix();
	_phongShadingVert.projection = &_camera.ProjectionMatrix();
	for (int i = 0; i < _sphere.submeshes.size(); i++) {
		const vector<Vec3f>& vertices = _sphere.submeshes[i].vertices;
		//const vector<Vec4f>& colors = _sphere.submeshes[i].colors;
		const vector<Vec2f>& uvs = _sphere.submeshes[i].uvs;
		const vector<Vec3f>& normals = _sphere.submeshes[i].normals;
		const vector<uint32_t>& indices = _sphere.submeshes[i].indices;
		for (int j = 0; j < _sphere.submeshes[i].indices.size(); j += 3) {
			Vec4f ndc0, ndc1, ndc2;
			Vec2f u0, u1, u2;
			Vec3f n0, n1, n2;
			Vec3f worldPos0, worldPos1, worldPos2;

			_phongShadingVert.Main(vertices[indices[j]], uvs[indices[j]], normals[indices[j]], ndc0);
			n0 = _phongShadingVert.voutNormal;
			worldPos0 = _phongShadingVert.voutWorldPos;

			_phongShadingVert.Main(vertices[indices[j + 1]], uvs[indices[j + 1]], normals[indices[j + 1]], ndc1);
			n1 = _phongShadingVert.voutNormal;
			worldPos1 = _phongShadingVert.voutWorldPos;

			_phongShadingVert.Main(vertices[indices[j + 2]], uvs[indices[j + 2]], normals[indices[j + 2]], ndc2);
			n2 = _phongShadingVert.voutNormal;
			worldPos2 = _phongShadingVert.voutWorldPos;

			ToNDC<float>(ndc0, ndc1, ndc2);

			Viewport(0, 0, context.Dimension().first, context.Dimension().second, n, f, ndc0);
			Viewport(0, 0, context.Dimension().first, context.Dimension().second, n, f, ndc1);
			Viewport(0, 0, context.Dimension().first, context.Dimension().second, n, f, ndc2);

			LightingVertex<float> v0 = { ndc0, uvs[indices[j]], n0, worldPos0 };
			LightingVertex<float> v1 = { ndc1, uvs[indices[j + 1]], n1, worldPos1 };
			LightingVertex<float> v2 = { ndc2, uvs[indices[j + 2]], n2, worldPos2 };
			_phongShadingFrag.texture = &_sphere.GetTexture(0);
			Triangle(v0, v1, v2, &_phongShadingFrag);
		}
	}
}

void PhongShading::LeftKeyDownTask()
{
	_phongShadingFrag.light.position.x -= 0.03125f;
}

void PhongShading::RightKeyDownTask()
{
	_phongShadingFrag.light.position.x += 0.03125f;
}

void PhongShading::UpKeyDownTask()
{
	_phongShadingFrag.light.position.y += 0.03125f;
}

void PhongShading::DownKeyDownTask()
{
	_phongShadingFrag.light.position.y -= 0.03125f;
}

void PhongShading::CKeyDownTask()
{
	if (context.cullFaceMode == Context::CullFaceMode::NONE) {
		context.cullFaceMode = Context::CullFaceMode::BACK;
		std::cout << "CullFaceMode::BACK\n";
	}
	else if (context.cullFaceMode == Context::CullFaceMode::BACK) {
		context.cullFaceMode = Context::CullFaceMode::FRONT;
		std::cout << "CullFaceMode::FRONT\n";
	}
	else if (context.cullFaceMode == Context::CullFaceMode::FRONT) {
		context.cullFaceMode = Context::CullFaceMode::NONE;
		std::cout << "CullFaceMode::NONE\n";
	}
}

void PhongShading::DKeyDownTask()
{
	context.depthTest = !context.depthTest;
}
