#pragma once
#include "render/Grid.h"
#include <iostream>

class TextureResource {
public:
	TextureResource();
	~TextureResource();

	void bind(GLuint slot = 0) const;
	void unbind()const;
	void loadTextureURI(const std::string& path, int flip = 0);
private:
	GLuint rendererID;
	unsigned char* localBuffer;
	GLint width, height, bitsPerPixel;
};