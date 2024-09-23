#pragma once;
#include <memory>
#include "config.h"
#include "math/mat4.h"
#include "render/shaders/shaderResource.h"
#include "render/camera/camera.h"
#include "render/textures/textureResource.h"
#include "render/meshes/meshResource.h"
#include "render/meshes/meshTransform.h"

class GraphicsNode {
public:
	std::shared_ptr<MeshResource> mesh;
	std::shared_ptr<TextureResource> texture;
	std::shared_ptr<MeshTransform> transform;
	std::shared_ptr<ShaderResource> shader;
public:
	GraphicsNode();
	~GraphicsNode();

	void setMesh(const std::shared_ptr<MeshResource>& newMesh);
	void setTexture(const std::shared_ptr<TextureResource>& newTexture);
	void setTransform(const std::shared_ptr<MeshTransform>& newTransform);
	void setShader(const std::shared_ptr<ShaderResource>& newShader);

	std::shared_ptr<MeshResource> getMesh() const;
	std::shared_ptr<TextureResource> getTexture() const;
	std::shared_ptr<ShaderResource> getShader() const;
	MeshTransform getTransform() const;

	void draw(const Camera& camera);
};