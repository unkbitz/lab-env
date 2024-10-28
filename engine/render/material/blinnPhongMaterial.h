#pragma once
#include "config.h"
#include "render/material/material.h"
#include "render/shaders/shaderResource.h"
#include "render/textures/textureResource.h"
#include <memory>

class BlinnPhongMaterial : public Material {
public:
	BlinnPhongMaterial(	std::shared_ptr<ShaderResource> shader);
	~BlinnPhongMaterial();
	void Apply() const override;
	void setDiffuseTexture(std::shared_ptr<TextureResource> diffuse);
	void setSpecularTexture(std::shared_ptr<TextureResource> specular);
	void setMetallicTexture(std::shared_ptr<TextureResource> metallic);
	void setEmissiveTexture(std::shared_ptr<TextureResource> emissive);
	void setNormalMapTexture(std::shared_ptr<TextureResource> normalMap);
	void setOcclusionTexture(std::shared_ptr<TextureResource> occlusion);
	void setShininess(float shininess);
private:
	std::shared_ptr<ShaderResource> blinnPhongShader;
	std::shared_ptr<TextureResource> diffuseTexture;
	std::shared_ptr<TextureResource> specularTexture;
	std::shared_ptr<TextureResource> metallicTexture;
	std::shared_ptr<TextureResource> emissiveTexture;
	std::shared_ptr<TextureResource> normalMapTexture;
	std::shared_ptr<TextureResource> occlusionTexture;
	float shininess;
};

