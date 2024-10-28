#pragma once
#include "render/textures/textureResource.h"
#include "render/graphics/graphics.h"

class GLTFLoader {
public:
	GLTFLoader();
	~GLTFLoader();

	static std::shared_ptr<GraphicsNode> loadGLTFRootNode(const std::string& uri);
	static std::shared_ptr<GraphicsNode> loadGLTFNode(const fx::gltf::Document& document, int nodeIndex);
	static std::shared_ptr<TextureResource> loadTexture(const fx::gltf::Document& document, int textureIndex);
private:
};