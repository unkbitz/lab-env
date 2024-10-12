#pragma once
#include "config.h"
#include "render/material/material.h"
#include "render/shaders/shaderResource.h"
#include "render/textures/textureResource.h"
#include <memory>

class BlinnPhongMaterial : public Material {
public:
	BlinnPhongMaterial(
		std::shared_ptr<ShaderResource> shader);
	~BlinnPhongMaterial();
	void Apply() const override;
	void setDiffuseTexture(std::shared_ptr<TextureResource> diffuse);
	void setSpecularTexture(std::shared_ptr<TextureResource> specular);
	void setShininess(float shininess);
private:
	std::shared_ptr<ShaderResource> blinnPhongShader;
	std::shared_ptr<TextureResource> diffuseTexture;
	std::shared_ptr<TextureResource> specularTexture;
	float shininess;
};

