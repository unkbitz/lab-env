#pragma once
#include <memory>
#include "config.h"
#include "math/mat4.h"
#include "render/shaders/shaderResource.h"
#include "render/camera/camera.h"
#include "render/textures/textureResource.h"
#include "render/meshes/meshResource.h"
#include "render/meshes/meshTransform.h"
#include "render/lighting/lighting.h"
#include "render/material/blinnPhongMaterial.h"

class GraphicsNode {
public:
	GraphicsNode();
	~GraphicsNode();

	void setMesh(const std::shared_ptr<MeshResource>& newMesh);
	void setTexture(const std::shared_ptr<TextureResource>& newTexture);
	void setShader(const std::shared_ptr<ShaderResource>& newShader);
	void setMaterial(std::shared_ptr<Material> mat);
	void addChild(std::shared_ptr<GraphicsNode> child);

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
	void drawGeometry(Camera& camera);
	void draw(Camera& camera, Lighting& light);
	void drawDebugLights(Camera& camera);
	void setBaseColor(const vec4& color);
	void setUseDiffuseTexture(bool value);
private:
	std::shared_ptr<MeshResource> m_mesh;
	std::shared_ptr<TextureResource> m_texture;
	std::shared_ptr<ShaderResource> m_shader;
	std::shared_ptr<Material> m_material;
	std::vector<std::shared_ptr<GraphicsNode>> m_childNodes;

	vec4 m_position = vec4(0, 0, 0, 1);
	mat4 m_rotation;
	vec3 m_scale = vec3(1, 1, 1);

	vec4 baseColor = vec4(1, 1, 1, 1);
	bool useDiffuseTexture = true;
};