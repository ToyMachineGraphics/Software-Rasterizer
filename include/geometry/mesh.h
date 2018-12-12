#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED

#include "geometry/vec2.hpp"
#include "geometry/vec3.hpp"
#include "geometry/vec4.hpp"
#include "transform/transform.hpp"
#include "texture.h"
#include <vector>

namespace SoftwareRenderer {
namespace Geometry {

	using Geometry::Vec2f;
	using Geometry::Vec3f;
	using Geometry::Vec4f;
	using Transform::Matrix44f;
	using std::vector;

    class Mesh {
    public:
		Mesh() : drawType(Fill), _modelMatrix(1.0f) { /*_textures.reserve(2);*/ }
		Mesh(const Mesh& mesh) = delete;
		Mesh& operator=(const Mesh& mesh) = delete;
		Mesh(const Mesh&& mesh) = delete;
		Mesh& operator=(const Mesh&& mesh) = delete;
		~Mesh() { _textures.clear(); }

        enum DrawType {
            WireFrame,
            Fill
        };

		typedef struct SubMesh
		{
			vector<Vec3f> vertices;
			vector<Vec4f> colors;
			vector<Vec2f> uvs;
			vector<Vec3f> normals;
			vector<uint32_t> indices;
		} SubMesh;

		static void CreateCube(Mesh& result);
		static void CreateSphere(Mesh& result);

		DrawType drawType;
		vector<SubMesh> submeshes;

		void ResetModelMatrix();
		void Translate(float x, float y, float z);
		void AngleAxis(Vec3f axis, float angle);

		void LoadTexture(const char* filename);

		const Matrix44f& ModelMatrix() { return _modelMatrix; }
		const Texture& GetTexture(int index) { return _textures[index]; }
	private:
		Matrix44f _modelMatrix;
		vector<Texture> _textures;
    };
}}

#endif // MESH_H_INCLUDED
