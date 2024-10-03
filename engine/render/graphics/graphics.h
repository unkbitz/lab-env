#pragma once;
#include <memory>
#include "config.h"
#include "math/mat4.h"
#include "render/shaders/shaderResource.h"
#include "render/camera/camera.h"
#include "render/textures/textureResource.h"
#include "render/meshes/meshResource.h"
#include "render/meshes/meshTransform.h"
#include "render/lighting/lighting.h"

class GraphicsNode {
private:
	std::shared_ptr<MeshResource> m_mesh;
	std::shared_ptr<TextureResource> m_texture;
	std::shared_ptr<ShaderResource> m_shader;
public:
	GraphicsNode();
	~GraphicsNode();

	void setMesh(const std::shared_ptr<MeshResource>& newMesh);
	void setTexture(const std::shared_ptr<TextureResource>& newTexture);
	void setShader(const std::shared_ptr<ShaderResource>& newShader);

	std::shared_ptr<MeshResource> getMesh() const;
	std::shared_ptr<TextureResource> getTexture() const;
	std::shared_ptr<ShaderResource> getShader() const;
	mat4 getTransform() const;
	void setTransform(vec4 newPos, mat4 newRot);
	void setPosition(vec4 const newPos);
	vec4 getPosition();
	void setRotation(mat4 const newRot);
	mat4 getRotation();
	void setScale(vec3 const newScale);
	vec3 getScale();
	void draw(Camera& camera, Lighting& light);
};