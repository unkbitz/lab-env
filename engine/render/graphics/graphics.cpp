#pragma once
#include "config.h"
#include "graphics.h"

GraphicsNode::GraphicsNode() {
}

GraphicsNode::~GraphicsNode() {
}

void GraphicsNode::setMesh(const std::shared_ptr<MeshResource>& newMesh) {
	mesh = newMesh;
}

void GraphicsNode::setTexture(const std::shared_ptr<TextureResource>& newTexture) {
	texture = newTexture;
}

void GraphicsNode::setTransform(const std::shared_ptr<MeshTransform>& newTransform) {
	transform = newTransform;
}

void GraphicsNode::setShader(const std::shared_ptr<ShaderResource>& newShader) {
	shader = newShader;
}

std::shared_ptr<MeshResource> GraphicsNode::getMesh() const
{
	return mesh;
}

std::shared_ptr<TextureResource> GraphicsNode::getTexture() const
{
	return texture;
}

std::shared_ptr<ShaderResource> GraphicsNode::getShader() const
{
	return shader;
}

MeshTransform GraphicsNode::getTransform() const
{
	return *transform;
}

void GraphicsNode::draw(const Camera& camera) {
	shader->bind();
	shader->setUniformMat4("u_ViewProjection", camera.getprojectionMatrix(), shader->getProgram());
	shader->setUniformMat4("u_Model", transform->getTransformMatrix(), shader->getProgram());

	texture->bind(0);

	mesh->bindBuffers();
	mesh->drawMesh();

	texture->unbind();
	shader->unbind();
}
