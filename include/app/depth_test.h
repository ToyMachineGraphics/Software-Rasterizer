#include "scene.h"

#include "camera.hpp"
#include "geometry/mesh.h"
#include "shader/base_color_vert.h"
#include "shader/base_color_frag.h"
#include "shader/base_uv_vert.h"
#include "shader/base_uv_frag.h"

#include <vector>

using SoftwareRenderer::Camera;
using SoftwareRenderer::Geometry::Mesh;
using SoftwareRenderer::Shader::BaseColorVert;
using SoftwareRenderer::Shader::BaseColorFrag;
using SoftwareRenderer::Shader::BaseUVVert;
using SoftwareRenderer::Shader::BaseUVFrag;

class DepthTest : public Scene
{
public:
	DepthTest();
private:
	void RenderTask() override;
	void LeftKeyDownTask() override {}
	void RightKeyDownTask() override {}
	void UpKeyDownTask() override {}
	void DownKeyDownTask() override {}
	void CKeyDownTask() override;
	void DKeyDownTask() override;

	Camera<float> _camera;

	Mesh _mesh;
	Mesh _mesh1;
	Mesh _mesh2;
	BaseColorVert _baseColorVert;
	BaseColorFrag _baseColorFrag;
	BaseUVVert _baseUVVert;
	BaseUVFrag _baseUVFrag;

	const float _baseMoveSpeed;
};