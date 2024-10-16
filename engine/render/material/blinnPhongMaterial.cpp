#pragma once
#include "config.h"
#include "render/material/blinnPhongMaterial.h"

BlinnPhongMaterial::BlinnPhongMaterial(
	std::shared_ptr<ShaderResource> shader) : 
	blinnPhongShader(shader) {}

BlinnPhongMaterial::~BlinnPhongMaterial() {}

void BlinnPhongMaterial::Apply() const {
	blinnPhongShader->bind();
	blinnPhongShader->setUniform1f("material.shininess", shininess);

	if (diffuseTexture) {
		diffuseTexture->bind(0);
		blinnPhongShader->setUniform1i("material.diffuse", 0);
	}

	if (specularTexture) {
		specularTexture->bind(1);
		blinnPhongShader->setUniform1i("material.specular", 1);
	}
}

void BlinnPhongMaterial::setDiffuseTexture(std::shared_ptr<TextureResource> diffuse) {
	this->diffuseTexture = diffuse;
}

void BlinnPhongMaterial::setSpecularTexture(std::shared_ptr<TextureResource> specular) {
	this->specularTexture = specular;
}

void BlinnPhongMaterial::setShininess(float shininess) {
	this->shininess = shininess;
}
