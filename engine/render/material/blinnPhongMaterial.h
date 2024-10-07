#pragma once
#include "config.h"
#include "render/material/material.h"
#include "render/shaders/shaderResource.h"

class BlinnPhongMaterial : public Material {
public:
	BlinnPhongMaterial(std::shared_ptr<ShaderResource> shader);
	~BlinnPhongMaterial();
	void Apply() const override;
private:
	std::shared_ptr<ShaderResource> blinnPhongShader;
};

