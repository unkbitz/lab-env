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
	std::shared_ptr<MeshResource> m_mesh;
	std::shared_ptr<TextureResource> m_texture;
	std::shared_ptr<MeshTransform> m_transform;
	std::shared_ptr<ShaderResource> m_shader;
public:
	GraphicsNode();
	GraphicsNode(std::shared_ptr<MeshTransform> m_tranform);
	~GraphicsNode();

	void setMesh(const std::shared_ptr<MeshResource>& newMesh);
	void setTexture(const std::shared_ptr<TextureResource>& newTexture);
	void setTransform(const std::shared_ptr<MeshTransform>& newTransform);
	void setShader(const std::shared_ptr<ShaderResource>& newShader);

	std::shared_ptr<MeshResource> getMesh() const;
	std::shared_ptr<TextureResource> getTexture() const;
	std::shared_ptr<ShaderResource> getShader() const;
	std::shared_ptr<MeshTransform> getTransform() const;
	void setTransform(vec4 newPos, mat4 newRot);
	void draw(const Camera& camera);
};