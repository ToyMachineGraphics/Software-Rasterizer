#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <cstdint>

namespace SoftwareRenderer
{
	class Texture
	{
	public:
		Texture();
		Texture(const Texture& texure) = delete;
		Texture(Texture&& texure);
		Texture& operator=(Texture&& other) = delete;
		Texture& operator=(const Texture& other) = delete;
		~Texture();

		bool LoadFile(const char* filename);

		int Width() const { return _width; }
		int Height() const { return _height; }
		int ChannelCount() const { return _channelCount; }
		const uint8_t* Pixels() const { return _pixels; }
	private:
		int _width, _height;
		int _channelCount;
		uint8_t* _pixels;
	};
}

#endif