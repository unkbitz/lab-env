#pragma once
#include "config.h"
#include "render/material/blinnPhongMaterial.h"

BlinnPhongMaterial::BlinnPhongMaterial(std::shared_ptr<ShaderResource> shader) : blinnPhongShader(shader) {}

BlinnPhongMaterial::~BlinnPhongMaterial() {}

void BlinnPhongMaterial::Apply() const {
	blinnPhongShader->bind();
}
