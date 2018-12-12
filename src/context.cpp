#include "context.h"
#include "lib/SDL_wrapper.h"
#include <cassert>
#include <algorithm>

namespace SoftwareRenderer
{
	Context::Context() : _width(0), _height(0),
						 _n(INT32_MIN), _f(INT32_MIN),
						 _zBuffer(nullptr), depthTest(true)
	{
		onCreateWindow = [&](int width, int height) {
			SetDimension(width, height);
			CreateDepthBuffer();
			SetDepthRange(1, 0);
		};
	}

    Context::~Context()
    {
        delete[] _zBuffer;
    }

    void Context::SetDimension(int width, int height)
    {
        _width = width;
        _height = height;
    }

    void Context::CreateDepthBuffer()
    {
        assert(_zBuffer == nullptr);
        _zBuffer = new float[_width * _height];
    }


	void Context::ClearDepthBuffer()
	{
		std::fill(_zBuffer, _zBuffer + _width * _height, _f);
	}
}
