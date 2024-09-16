#pragma once
#include "textureResource.h"
#include "stb_image.h"


texture::TextureResource::TextureResource(const std::string& path) 
	: rendererID(0), filePath(path), LocalBuffer(nullptr), 
	width(0), height(0), bitsPerPixel(0)
{
	stbi_set_flip_vertically_on_load(1); //flip image
	LocalBuffer = stbi_load(path.c_str(), &width, &height, &bitsPerPixel, 0);

	glGenTextures(1, &rendererID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, LocalBuffer);
	
	if (LocalBuffer) {
		stbi_image_free(LocalBuffer);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}
texture::TextureResource::~TextureResource() {
	glDeleteTextures(1, &rendererID);
}

void texture::TextureResource::bind(GLuint slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void texture::TextureResource::unbind()const {
	glBindTexture(GL_TEXTURE_2D, 0);
}

