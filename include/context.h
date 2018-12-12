#ifndef CONTEXT_H_INCLUDED
#define CONTEXT_H_INCLUDED

#include <utility>

using std::pair;

namespace SoftwareRenderer
{
    class Context
    {
    public:
		Context();
        Context(const Context&) = delete;
        Context& operator=(const Context&) = delete;
		~Context();

        void SetDimension(int width, int height);

        void CreateDepthBuffer();
		void SetDepthRange(float n, float f) { _n = n, _f = f; }
		void DepthRange(float& n, float& f) { n = _n, f = _f; }
		float& DepthValueAt(int x, int y) { return _zBuffer[y * _height + x]; }
		void ClearDepthBuffer();

		pair<int, int> Dimension() { return pair<int, int>(_width, _height); }
		enum CullFaceMode {
			NONE, BACK, FRONT
		};
		CullFaceMode cullFaceMode = CullFaceMode::NONE;
		bool depthTest;
    private:
        int _width, _height;

		float _n, _f;
        float* _zBuffer;
    };
}

extern SoftwareRenderer::Context context;

#endif // CONTEXT_H_INCLUDED
