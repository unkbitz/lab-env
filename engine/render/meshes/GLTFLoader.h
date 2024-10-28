#pragma once
#include "render/textures/textureResource.h"
#include "render/graphics/graphics.h"

class GLTFLoader {
public:
	GLTFLoader();
	~GLTFLoader();

	static std::shared_ptr<GraphicsNode> loadGLTFRootNode(const std::string& uri, int imageFlip, string folderPath);
	static std::shared_ptr<GraphicsNode> loadGLTFNode(const fx::gltf::Document& document, int nodeIndex, int imageFlip, string folderPath);
	static std::shared_ptr<TextureResource> loadTexture(const fx::gltf::Document& document, int textureIndex, int imageFlip, string folderPath);
private:
};