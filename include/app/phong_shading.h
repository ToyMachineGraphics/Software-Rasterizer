#ifndef PHONG_SHADING_H_INCLUDED
#define PHONG_SHADING_H_INCLUDED

#include "scene.h"
#include "camera.hpp"
#include "geometry/mesh.h"
#include "shader/phong_shading_vert.h"
#include "shader/phong_shading_frag.h"

using SoftwareRenderer::Camera;
using SoftwareRenderer::Geometry::Mesh;
using SoftwareRenderer::Shader::PhongShadingVert;
using SoftwareRenderer::Shader::PhongShadingFrag;

class PhongShading : public Scene
{
public:
	PhongShading();

private:
	void RenderTask() override;
	void LeftKeyDownTask() override;
	void RightKeyDownTask() override;
	void UpKeyDownTask() override;
	void DownKeyDownTask() override;
	void CKeyDownTask() override;
	void DKeyDownTask() override;

	Camera<float> _camera;

	Mesh _sphere;

	PhongShadingVert _phongShadingVert;
	PhongShadingFrag _phongShadingFrag;
};

#endif