#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "image/stb_image.h"

namespace SoftwareRenderer
{
	Texture::Texture() : _pixels(nullptr)
	{
	}

	Texture::Texture(Texture&& texure)
	{
		_pixels = texure._pixels;
		_width = texure._width;
		_height = texure._height;
		_channelCount = texure._channelCount;
		texure._pixels = nullptr;
		texure._width = -1;
		texure._height = -1;
		texure._channelCount = -1;
	}

	Texture::~Texture()
	{
		if (_pixels) {
			stbi_image_free(_pixels);
			_pixels = nullptr;
		}
	}

	bool Texture::LoadFile(const char* filename)
	{
		_pixels = stbi_load(filename, &_width, &_height, &_channelCount, 0);
		return (_pixels != nullptr);
	}
}