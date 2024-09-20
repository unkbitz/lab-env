#pragma once
#include "render/Grid.h"
#include <iostream>

class TextureResource {
private:
	GLuint rendererID;
	unsigned char* localBuffer;
	GLint width, height, bitsPerPixel;

public:
	TextureResource();
	~TextureResource();

	void bind(GLuint slot = 0) const;
	void unbind()const;
	void load(const std::string& path);

};

