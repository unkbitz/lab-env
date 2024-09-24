#pragma once
#include "config.h"
#include "graphics.h"

GraphicsNode::GraphicsNode() 
	: m_transform(std::make_shared<MeshTransform>()), 
	m_mesh(nullptr),
	m_texture(nullptr), 
	m_shader(nullptr) {}

GraphicsNode::GraphicsNode(std::shared_ptr<MeshTransform> transform)
	: m_transform(transform), 
	m_mesh(nullptr), 
	m_texture(nullptr), 
	m_shader(nullptr) {}

GraphicsNode::~GraphicsNode() {
}

void GraphicsNode::setMesh(const std::shared_ptr<MeshResource>& newMesh) {
	m_mesh = newMesh;
}

void GraphicsNode::setTexture(const std::shared_ptr<TextureResource>& newTexture) {
	m_texture = newTexture;
}

void GraphicsNode::setTransform(const std::shared_ptr<MeshTransform>& newTransform) {
	m_transform = newTransform;
}

void GraphicsNode::setShader(const std::shared_ptr<ShaderResource>& newShader) {
	m_shader = newShader;
}

std::shared_ptr<MeshResource> GraphicsNode::getMesh() const {
	return m_mesh;
}

std::shared_ptr<TextureResource> GraphicsNode::getTexture() const {
	return m_texture;
}

std::shared_ptr<ShaderResource> GraphicsNode::getShader() const {
	return m_shader;
}

std::shared_ptr<MeshTransform> GraphicsNode::getTransform() const {
	return m_transform;
}

void GraphicsNode::setTransform(vec4 newPos, mat4 newRot) {
	if (m_transform == nullptr)	{
		std::cout << "Input MeshTransform is invalid" << std::endl;
		assert(false);
	}
	m_transform->setPosition(newPos);
	m_transform->setRotation(newRot);
}

void GraphicsNode::draw(const Camera& camera) {
	m_shader->bind();
	m_shader->setUniformMat4("u_ViewProjection", camera.getprojectionMatrix(), m_shader->getProgram());
	m_shader->setUniformMat4("u_Model", m_transform->getTransformMatrix(), m_shader->getProgram());

	m_texture->bind(0);

	//m_mesh->bindBuffers();
	m_mesh->drawMesh();

	m_texture->unbind();
	m_shader->unbind();
}
