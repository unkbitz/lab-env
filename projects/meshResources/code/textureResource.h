#pragma once
#include "render/Grid.h"
#include <iostream>

namespace texture {

	class TextureResource {
	private:
		GLuint rendererID;
		std::string filePath;
		unsigned char* LocalBuffer;
		GLint width, height, bitsPerPixel;

	public:
		TextureResource(const std::string& path);
		~TextureResource();

		void bind(GLuint slot = 0) const;
		void unbind()const;

	};
}
