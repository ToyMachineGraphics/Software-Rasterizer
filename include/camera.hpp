#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "transform/matrix.hpp"

namespace SoftwareRenderer {

	using Transform::Vec3;
	using Transform::Matrix44;

	template <typename T>
	class Camera
	{
	public:
		void LookAt(T eyeX, T eyeY, T eyeZ,
					T centerX, T centerY, T centerZ,
					T upX, T upY, T upZ)
		{
			Vec3<T> viewDir = Vec3<T>(centerX - eyeX, centerY - eyeY, centerZ - eyeZ);
			viewDir.Normalize();
			Vec3<T> rightDir = viewDir.Cross(Vec3<T>(upX, upY, upZ));
			rightDir.Normalize();
			Vec3<T> upDir = rightDir.Cross(viewDir);
			_viewMatrix[0][0] = rightDir.x;
			_viewMatrix[1][0] = rightDir.y;
			_viewMatrix[2][0] = rightDir.z;
			_viewMatrix[3][0] = 0;
			_viewMatrix[0][1] = upDir.x;
			_viewMatrix[1][1] = upDir.y;
			_viewMatrix[2][1] = upDir.z;
			_viewMatrix[3][1] = 0;
			_viewMatrix[0][2] = -viewDir.x;
			_viewMatrix[1][2] = -viewDir.y;
			_viewMatrix[2][2] = -viewDir.z;
			_viewMatrix[3][2] = 0;
			_viewMatrix[0][3] = 0;
			_viewMatrix[1][3] = 0;
			_viewMatrix[2][3] = 0;
			_viewMatrix[3][3] = 1;
			T tmp[] = { 1, 0, 0, 0,
						0, 1, 0, 0,
						0, 0, 1, 0,
						-eyeX, -eyeY, -eyeZ, 1 };
			Matrix44<T> inverseEye(tmp);
			_viewMatrix = _viewMatrix * inverseEye;
			_cameraMatrix = _projectionMatrix * _viewMatrix;
		}

		void Frustum(T b, T t, T l, T r, T n, T f)
		{
			_projectionMatrix[0][0] = 2 * n / (r - l);
			_projectionMatrix[0][1] = 0;
			_projectionMatrix[0][2] = 0;
			_projectionMatrix[0][3] = 0;
			_projectionMatrix[1][0] = 0;
			_projectionMatrix[1][1] = 2 * n / (t - b);
			_projectionMatrix[1][2] = 0;
			_projectionMatrix[1][3] = 0;
			_projectionMatrix[2][0] = (r + l) / (r - l);
			_projectionMatrix[2][1] = (t + b) / (t - b);
			_projectionMatrix[2][2] = -(f + n) / (f - n);
			_projectionMatrix[2][3] = -1;
			_projectionMatrix[3][0] = 0;
			_projectionMatrix[3][1] = 0;
			_projectionMatrix[3][2] = -2 * f * n / (f - n);
			_projectionMatrix[3][3] = 0;

			_cameraMatrix = _projectionMatrix * _viewMatrix;
		}

		void Perspective(T fovy, T aspect, T zNear, T zFar)
		{
			T t = (T)tan(fovy * 0.5 * 3.141592654 / 180.0) * zNear;
			T b = -t;
			T r = aspect * t;
			T l = -r;
			Frustum(b, t, l, r, zNear, zFar);
		}

		const Matrix44<T>& ViewMatrix() { return _viewMatrix; }
		const Matrix44<T>& ProjectionMatrix() { return _projectionMatrix; }
		const Matrix44<T>& CameraMatrix() { return _cameraMatrix; }
	private:
		Matrix44<T> _viewMatrix;
		Matrix44<T> _projectionMatrix;

		Matrix44<T> _cameraMatrix;
	};
}

#endif // CAMERA_H_INCLUDED