#pragma once
#include "config.h"
#include "graphics.h"

GraphicsNode::GraphicsNode() 
	: m_mesh(nullptr),
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

void GraphicsNode::draw(const Camera& camera) {
	m_shader->bind();
	m_shader->setUniformMat4("u_ViewProjection", camera.getprojectionMatrix(), m_shader->getProgram());
	m_shader->setUniformMat4("u_Model", m_mesh->getTransform(), m_shader->getProgram());

	m_texture->bind(0);

	//m_mesh->bindBuffers();
	m_mesh->drawMesh();

	m_texture->unbind();
	m_shader->unbind();
}
