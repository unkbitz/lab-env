#pragma once
#include "config.h"
#include "graphics.h"

GraphicsNode::GraphicsNode() 
	: m_mesh(nullptr),
	m_texture(nullptr), 
	m_shader(nullptr) {}

GraphicsNode::~GraphicsNode() {}

void GraphicsNode::setMesh(const std::shared_ptr<MeshResource>& newMesh) {
	m_mesh = newMesh;
}

void GraphicsNode::setTexture(const std::shared_ptr<TextureResource>& newTexture) {
	m_texture = newTexture;
}

void GraphicsNode::setShader(const std::shared_ptr<ShaderResource>& newShader) {
	m_shader = newShader;
}

std::shared_ptr<MeshResource> GraphicsNode::getMesh() const {
	if (m_mesh == nullptr) {
		std::cout << "m_mesh is nullptr" << std::endl;
		assert(false);
	}
	return m_mesh;
}

std::shared_ptr<TextureResource> GraphicsNode::getTexture() const {
	if (m_texture == nullptr) {
		std::cout << "m_mesh is nullptr" << std::endl;
		assert(false);
	}
	return m_texture;
}

std::shared_ptr<ShaderResource> GraphicsNode::getShader() const {
	if (m_shader == nullptr) {
		std::cout << "m_mesh is nullptr" << std::endl;
		assert(false);
	}
	return m_shader;
}

mat4 GraphicsNode::getTransform() const {
	if (m_mesh == nullptr) {
		std::cout << "m_mesh is nullptr" << std::endl;
		assert(false);
	}
	return m_mesh->getTransform();
}

//is this method needed?
void GraphicsNode::setTransform(vec4 newPos, mat4 newRot) {
	if (m_mesh == nullptr)	{
		std::cout << "Input MeshTransform is invalid" << std::endl;
		assert(false);
	}
	m_mesh->setPosition(newPos);
	m_mesh->setRotation(newRot);
}

void GraphicsNode::setPosition(vec4 const newPos) {
	if (m_mesh == nullptr) {
		std::cout << "Input MeshTransform is invalid" << std::endl;
		assert(false);
	}
	m_mesh->setPosition(newPos);
}

vec4 GraphicsNode::getPosition() {
	if (m_mesh == nullptr) {
		std::cout << "Input MeshTransform is invalid" << std::endl;
		assert(false);
	}
	return m_mesh->getPosition();
}

void GraphicsNode::setRotation(mat4 const newRot) {
	if (m_mesh == nullptr) {
		std::cout << "Input MeshTransform is invalid" << std::endl;
		assert(false);
	}
	m_mesh->setRotation(newRot);
}
mat4 GraphicsNode::getRotation() {
	if (m_mesh == nullptr) {
		std::cout << "Input MeshTransform is invalid" << std::endl;
		assert(false);
	}
	return m_mesh->getRotation();
}

void GraphicsNode::setScale(vec3 const newScale) {
	if (m_mesh == nullptr) {
		std::cout << "Input MeshTransform is invalid" << std::endl;
		assert(false);
	}
	m_mesh->setScale(newScale);
}

vec3 GraphicsNode::getScale() {
	if (m_mesh == nullptr) {
		std::cout << "Input MeshTransform is invalid" << std::endl;
		assert(false);
	}
	return m_mesh->getScale();
}

void GraphicsNode::draw(Camera& camera, Lighting& light) {
	m_shader->bind();
	m_shader->setUniformMat4("u_ViewProjection", camera.getViewProjectionMatrix(), m_shader->getProgram());
	m_shader->setUniformMat4("u_Model", m_mesh->getTransform(), m_shader->getProgram());
	// Set the lighting-related uniforms for the Blinn-Phong shader
	m_shader->setUniform3f("u_pointLightPos", light.getPointLightPos().x, light.getPointLightPos().y, light.getPointLightPos().z);
	m_shader->setUniform3f("u_ViewPos", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
	m_shader->setUniform3f("u_pointLightColor", light.getPointLightColor().x, light.getPointLightColor().y, light.getPointLightColor().z);
	m_shader->setUniform1f("u_pointLightIntensity", light.getPointLightIntensity());
	m_shader->setUniform3f("u_directionalLightDir", light.getDirektionalLightDir().x, light.getDirektionalLightDir().y, light.getDirektionalLightDir().z);
	m_shader->setUniform3f("u_directionalLightColor", light.getDirektionalLightColor().x, light.getDirektionalLightColor().y, light.getDirektionalLightColor().z);
	m_shader->setUniform1f("u_directionalLightIntensity", light.getDirektionalLightIntensity());
	m_texture->bind(0);

	//m_mesh->bindBuffers();
	m_mesh->drawMesh();

	m_texture->unbind();
	m_shader->unbind();
}
