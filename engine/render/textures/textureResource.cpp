#pragma once
#include "config.h"
#include "textureResource.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


TextureResource::TextureResource() 
	: rendererID(0), localBuffer(nullptr), 
	width(0), height(0), bitsPerPixel(0) {}

TextureResource::~TextureResource() {
	glDeleteTextures(1, &rendererID);
}

void TextureResource::bind(GLuint slot) const {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, rendererID);
}

void TextureResource::unbind()const {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextureResource::load(const std::string& path) {
	localBuffer = stbi_load(path.c_str(), &width, &height, &bitsPerPixel, 4);
	glGenTextures(1, &rendererID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);

	if (localBuffer) {
		stbi_image_free(localBuffer);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}