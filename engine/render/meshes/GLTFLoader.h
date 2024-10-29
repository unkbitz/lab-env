#pragma once
#include "render/textures/textureResource.h"
#include "render/graphics/graphics.h"

class GLTFLoader {
public:
	GLTFLoader();
	~GLTFLoader();

	static std::shared_ptr<GraphicsNode> loadGLTFRootNode(
		const std::string& uri, 
		string folderPath, 
		string shaderPath, 
		float shininess = 32.0f,
		int imageFlip = 0);

	static std::shared_ptr<GraphicsNode> loadGLTFNode(
		const fx::gltf::Document& document, 
		int nodeIndex, 
		string folderPath, 
		string shaderPath, 
		float shininess = 32.0f,
		int imageFlip = 0);

	static std::shared_ptr<TextureResource> loadTexture(
		const fx::gltf::Document& document, 
		int textureIndex, 
		string folderPath, 
		int imageFlip = 0);
private:
};