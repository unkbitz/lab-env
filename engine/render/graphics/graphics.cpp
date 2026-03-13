#pragma once
#include "config.h"
#include "graphics.h"
#include <memory>

GraphicsNode::GraphicsNode() 
	: m_mesh(nullptr),
	m_texture(nullptr), 
	m_shader(nullptr){}

GraphicsNode::~GraphicsNode() {}

void GraphicsNode::setMesh(const std::shared_ptr<MeshResource>& newMesh) {
	m_mesh = newMesh;
}

void GraphicsNode::setTexture(const std::shared_ptr<TextureResource>& newTexture) {
	m_texture = newTexture;
}

void GraphicsNode::setShader(const std::shared_ptr<ShaderResource>& newShader) {
	m_shader = newShader;
	for (auto& child : m_childNodes) {
		child->setShader(newShader);
	}
}

void GraphicsNode::setMaterial(std::shared_ptr<Material> newMaterial) {
	//if (m_mesh == nullptr) {
	//	std::cout << "Input MeshTransform is invalid" << std::endl;
	//	assert(false);
	//}
	//m_mesh->setMaterial(newMaterial);
	m_material = newMaterial;
}

void GraphicsNode::addChild(std::shared_ptr<GraphicsNode> child) {
	m_childNodes.push_back(child);
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
	//if (m_mesh == nullptr) {
	//	std::cout << "m_mesh is nullptr" << std::endl;
	//	assert(false);
	//}
	//return m_mesh->getTransform();

	mat4 T = translation(m_position.x, m_position.y, m_position.z);
	mat4 S = scaling(m_scale);
	return T * m_rotation * S;
}

//is this method needed?
void GraphicsNode::setTransform(vec4 newPos, mat4 newRot) {
	//if (m_mesh == nullptr)	{
	//	std::cout << "Input MeshTransform is invalid" << std::endl;
	//	assert(false);
	//}
	//m_mesh->setPosition(newPos);
	//m_mesh->setRotation(newRot);
	m_position = newPos;
	m_rotation = newRot;
}

void GraphicsNode::setPosition(vec4 const newPos)
{
	//if (m_mesh == nullptr)
	//{
	//	std::cout << "Input MeshTransform is invalid" << std::endl;
	//	assert(false);
	//}
	//m_mesh->setPosition(newPos);
	m_position = newPos;
	for (auto& child : m_childNodes)
	{
		child->setPosition(newPos);
	}
}

vec4 GraphicsNode::getPosition()
{
	//if (m_mesh == nullptr)
	//{
	//	std::cout << "Input MeshTransform is invalid" << std::endl;
	//	assert(false);
	//}
	return m_position;
}

void GraphicsNode::setRotation(mat4 const newRot)
{
	//if (m_mesh == nullptr)
	//{
	//	std::cout << "Input MeshTransform is invalid" << std::endl;
	//	assert(false);
	//}
	//m_mesh->setRotation(newRot);
	m_rotation = newRot;

	for (auto& child : m_childNodes) {
		child->setRotation(newRot);
	}
}
mat4 GraphicsNode::getRotation()
{
	//if (m_mesh == nullptr)
	//{
	//	std::cout << "Input MeshTransform is invalid" << std::endl;
	//	assert(false);
	//}
	//return m_mesh->getRotation();
	return m_rotation;
}

void GraphicsNode::setScale(vec3 const newScale)
{
	//if (m_mesh == nullptr)
	//{
	//	std::cout << "Input MeshTransform is invalid" << std::endl;
	//	assert(false);
	//}

	//m_mesh->setScale(newScale);

	m_scale = newScale;

	for (auto& child : m_childNodes)
	{
		child->setScale(newScale);
	}
}

vec3 GraphicsNode::getScale()
{
	//if (m_mesh == nullptr)
	//{
	//	std::cout << "Input MeshTransform is invalid" << std::endl;
	//	assert(false);
	//}
	//return m_mesh->getScale();
	return m_scale;
}

void GraphicsNode::drawGeometry(Camera& camera)
{
	m_shader->bind();
	m_shader->setUniformMat4("u_ViewProjection", camera.getViewProjectionMatrix(), m_shader->getProgram());
	m_shader->setUniformMat4("u_Model", this->getTransform(), m_shader->getProgram());

	auto blinnMat = std::dynamic_pointer_cast<BlinnPhongMaterial>(m_material);
	if (!blinnMat)
	{
		std::cout << "No blinn material on node\n";
	}
	else
	{
		m_shader->setUniform1i("useDiffuseTexture", 0);
		m_shader->setUniform1i("useSpecularTexture", 0);

		m_shader->setUniform4fv("baseColor", this->baseColor, m_shader->getProgram());
		m_shader->setUniform4fv("specularColor", vec4(1.0f, 1.0f, 1.0f, 1.0f), m_shader->getProgram());

		auto diffuse = blinnMat->getDiffuseTexture();
		auto specular = blinnMat->getSpecularTexture();

		if (diffuse)
		{
			diffuse->bind(0);
			m_shader->setUniform1i("diffuseTex", 0);
			m_shader->setUniform1i("useDiffuseTexture", 1);
		}
		else
		{
			m_shader->setUniform1i("useDiffuseTexture", 0);
			m_shader->setUniform4fv("baseColor", this->baseColor, m_shader->getProgram());
		}


		if (specular)
		{
			specular->bind(1);
			m_shader->setUniform1i("specularTex", 1);
			m_shader->setUniform1i("useSpecularTexture", 1);
		}
		else
		{
			m_shader->setUniform1i("useSpecularTexture", 0);
			m_shader->setUniform4fv("specularColor", vec4(1.0f, 1.0f, 1.0f, 1.0f), m_shader->getProgram());
		}
	}


	m_mesh->drawMesh();
	m_shader->unbind();

	for (auto& child : m_childNodes) {
		child->drawGeometry(camera);
	}
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
	m_shader->setUniform3f("u_directionalLightDir", light.getDirectionalLightDir().x, light.getDirectionalLightDir().y, light.getDirectionalLightDir().z);
	m_shader->setUniform3f("u_directionalLightColor", light.getDirectionalLightColor().x, light.getDirectionalLightColor().y, light.getDirectionalLightColor().z);
	m_shader->setUniform1f("u_directionalLightIntensity", light.getDirectionalLightIntensity());

	m_mesh->drawMesh();
	m_shader->unbind();

	for (auto& child : m_childNodes) {
		child->draw(camera, light);
	}
}

void GraphicsNode::drawDebugLights(Camera& camera)
{
	m_shader->bind();
	m_shader->setUniformMat4("u_ViewProjection", camera.getViewProjectionMatrix(), m_shader->getProgram());
	m_shader->setUniformMat4("u_Model", this->getTransform(), m_shader->getProgram());
	m_shader->setUniform4fv("u_Color", this->baseColor, m_shader->getProgram());

	m_mesh->drawMesh();
	m_shader->unbind();

	for (auto& child : m_childNodes)
	{
		child->drawDebugLights(camera);
	}
}


void GraphicsNode::setBaseColor(const vec4& color)
{
	baseColor = color;
}

void GraphicsNode::setUseDiffuseTexture(bool value)
{
	useDiffuseTexture = value;
}
