#include "app/depth_test.h"
#include "geometry/vec2.hpp"
#include "geometry/vec3.hpp"
#include "geometry/vec4.hpp"
#include "rasterize/rasterize.h"
#include "transform/matrix.hpp"
#include "context.h"

#include <ctime>

using SoftwareRenderer::Geometry::Vec2f;
using SoftwareRenderer::Geometry::Vec3f;
using SoftwareRenderer::Geometry::Vec4f;

using namespace SoftwareRenderer::Rasterize;
using namespace SoftwareRenderer::Transform;
using std::vector;

#include <iostream>

DepthTest::DepthTest() : _baseMoveSpeed(1.0f)
{
	_camera.LookAt(0.25f, 1, 2, 0.25f, 0, 0, 0, 1, 0);
	_camera.Perspective(60, 1, 1.0f, 100.0f);
	Mesh::CreateCube(_mesh);
	_mesh.LoadTexture("asset/image/chessboard.png");

	Mesh::CreateCube(_mesh1);

	Mesh::CreateCube(_mesh2);
	_mesh2.LoadTexture("asset/image/checkermap.png");
}

void DepthTest::RenderTask()
{
	static uint16_t tick = 0;
	context.ClearDepthBuffer();
	float n, f;
	context.DepthRange(n, f);

	_mesh.ResetModelMatrix();
	double ratio = (double)tick / UINT16_MAX;
	double s = sin(ratio * 2 * 3.141592654);
	//std::cout << tick << "/" << UINT16_MAX << " " << ratio << " " << s << "\n";
	_mesh.Translate(0.0f, 0.0f, s);
	tick += CLOCKS_PER_SEC;

	for (int i = 0; i < _mesh.submeshes.size(); i++) {
		const vector<Vec3f>& vertices = _mesh.submeshes[i].vertices;
		//const vector<Vec4f>& colors = _mesh.submeshes[i].colors;
		const vector<Vec2f>& uvs = _mesh.submeshes[i].uvs;
		//const vector<Vec3f>& normals = _mesh.submeshes[i].normals;
		const vector<uint32_t>& indices = _mesh.submeshes[i].indices;
		for (int j = 0; j < _mesh.submeshes[i].indices.size(); j += 3) {
			Vec4f ndc0, ndc1, ndc2;
			Vec2f u0, u1, u2;
			_baseUVVert.model = &_mesh.ModelMatrix();
			_baseUVVert.view = &_camera.ViewMatrix();
			_baseUVVert.projection = &_camera.ProjectionMatrix();
			_baseUVVert.Main(vertices[indices[j]], uvs[indices[j]], ndc0, u0);
			_baseUVVert.Main(vertices[indices[j + 1]], uvs[indices[j + 1]], ndc1, u1);
			_baseUVVert.Main(vertices[indices[j + 2]], uvs[indices[j + 2]], ndc2, u2);
			ToNDC<float>(ndc0, ndc1, ndc2);

			Viewport(0, 0, context.Dimension().first, context.Dimension().second, n, f, ndc0);
			Viewport(0, 0, context.Dimension().first, context.Dimension().second, n, f, ndc1);
			Viewport(0, 0, context.Dimension().first, context.Dimension().second, n, f, ndc2);

			VertexPU<float> v0 = { ndc0, u0 };
			VertexPU<float> v1 = { ndc1, u1 };
			VertexPU<float> v2 = { ndc2, u2 };
			_baseUVFrag.texture = &_mesh.GetTexture(0);
			Triangle(v0, v1, v2, &_baseUVFrag);
		}
	}

	_mesh1.ResetModelMatrix();
	s = sin(ratio * 2 * 3.141592654 + CLOCKS_PER_SEC * 2.0f);
	_mesh1.Translate(0.5f, 0.0f, s);
	for (int i = 0; i < _mesh1.submeshes.size(); i++) {
		const vector<Vec3f>& vertices = _mesh1.submeshes[i].vertices;
		const vector<Vec4f>& colors = _mesh1.submeshes[i].colors;
		//const vector<Vec2f>& uvs = _mesh1.submeshes[i].uvs;
		//const vector<Vec3f>& normals = _mesh1.submeshes[i].normals;
		const vector<uint32_t>& indices = _mesh1.submeshes[i].indices;
		for (int j = 0; j < _mesh1.submeshes[i].indices.size(); j += 3) {
			Vec4f ndc0, ndc1, ndc2;
			Vec4f c0, c1, c2;
			_baseColorVert.model = &_mesh1.ModelMatrix();
			_baseColorVert.view = &_camera.ViewMatrix();
			_baseColorVert.projection = &_camera.ProjectionMatrix();
			_baseColorVert.Main(vertices[indices[j]], colors[indices[j]], ndc0, c0);
			_baseColorVert.Main(vertices[indices[j + 1]], colors[indices[j + 1]], ndc1, c1);
			_baseColorVert.Main(vertices[indices[j + 2]], colors[indices[j + 2]], ndc2, c2);
			ToNDC<float>(ndc0, ndc1, ndc2);

			Viewport(0, 0, context.Dimension().first, context.Dimension().second, n, f, ndc0);
			Viewport(0, 0, context.Dimension().first, context.Dimension().second, n, f, ndc1);
			Viewport(0, 0, context.Dimension().first, context.Dimension().second, n, f, ndc2);

			//Vec3f n0 = normals[indices[j]];
			//Vec3f n1 = normals[indices[j + 1]];
			//Vec3f n2 = normals[indices[j + 2]];
			VertexPC<float> v0 = { ndc0, c0 }; // Vec4f(n0.x, n0.y, n0.z, 1)
			VertexPC<float> v1 = { ndc1, c1 }; // Vec4f(n1.x, n1.y, n1.z, 1)
			VertexPC<float> v2 = { ndc2, c2 }; // Vec4f(n2.x, n2.y, n2.z, 1)
			Triangle(v0, v1, v2, &_baseColorFrag);
		}
	}

	_mesh2.ResetModelMatrix();
	s = sin(ratio * 2 * 3.141592654 + CLOCKS_PER_SEC * 0.5f);
	_mesh2.Translate(1.0f, 0.0f, s);
	for (int i = 0; i < _mesh2.submeshes.size(); i++) {
		const vector<Vec3f>& vertices = _mesh2.submeshes[i].vertices;
		//const vector<Vec4f>& colors = _mesh2.submeshes[i].colors;
		const vector<Vec2f>& uvs = _mesh2.submeshes[i].uvs;
		//const vector<Vec3f>& normals = _mesh2.submeshes[i].normals;
		const vector<uint32_t>& indices = _mesh2.submeshes[i].indices;
		for (int j = 0; j < _mesh2.submeshes[i].indices.size(); j += 3) {
			Vec4f ndc0, ndc1, ndc2;
			Vec2f u0, u1, u2;
			_baseUVVert.model = &_mesh2.ModelMatrix();
			_baseUVVert.view = &_camera.ViewMatrix();
			_baseUVVert.projection = &_camera.ProjectionMatrix();
			_baseUVVert.Main(vertices[indices[j]], uvs[indices[j]], ndc0, u0);
			_baseUVVert.Main(vertices[indices[j + 1]], uvs[indices[j + 1]], ndc1, u1);
			_baseUVVert.Main(vertices[indices[j + 2]], uvs[indices[j + 2]], ndc2, u2);
			ToNDC<float>(ndc0, ndc1, ndc2);

			Viewport(0, 0, context.Dimension().first, context.Dimension().second, n, f, ndc0);
			Viewport(0, 0, context.Dimension().first, context.Dimension().second, n, f, ndc1);
			Viewport(0, 0, context.Dimension().first, context.Dimension().second, n, f, ndc2);

			VertexPU<float> v0 = { ndc0, u0 };
			VertexPU<float> v1 = { ndc1, u1 };
			VertexPU<float> v2 = { ndc2, u2 };
			_baseUVFrag.texture = &_mesh2.GetTexture(0);
			Triangle(v0, v1, v2, &_baseUVFrag);
		}
	}
}

void DepthTest::CKeyDownTask()
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

void DepthTest::DKeyDownTask()
{
	context.depthTest = !context.depthTest;
}