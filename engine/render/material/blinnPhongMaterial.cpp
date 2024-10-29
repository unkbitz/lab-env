#pragma once
#include "config.h"
#include "render/material/blinnPhongMaterial.h"

BlinnPhongMaterial::BlinnPhongMaterial(std::shared_ptr<ShaderResource> shader, float shininess) : 
	blinnPhongShader(shader),
	shininess(32) {}

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
		blinnPhongShader->setUniform1i("material.hasSpecular", 1);
	}
	else {
		blinnPhongShader->setUniform1i("material.hasSpecular", 0);
	}
	
	if (metallicTexture) {
		metallicTexture->bind(2);
		blinnPhongShader->setUniform1i("material.metallic", 2);
		blinnPhongShader->setUniform1i("material.hasMetallic", 1);
	}
	else {
		blinnPhongShader->setUniform1i("material.hasMetallic", 0);
	}

	if (emissiveTexture) {
		emissiveTexture->bind(3);
		blinnPhongShader->setUniform1i("material.emissive", 3);
		blinnPhongShader->setUniform1i("material.hasEmissive", 1);
	}
	else {
		blinnPhongShader->setUniform1i("material.hasEmissive", 0);
	}

	if (normalMapTexture) {
		normalMapTexture->bind(4);
		blinnPhongShader->setUniform1i("material.normalMap", 4);
		blinnPhongShader->setUniform1i("material.hasNormalMap", 1);
	}
	else {
		blinnPhongShader->setUniform1i("material.hasNormalMap", 0);
	}

	if (occlusionTexture) {
		occlusionTexture->bind(5);
		blinnPhongShader->setUniform1i("material.occlusion", 5);
		blinnPhongShader->setUniform1i("material.hasOcclusion", 1);
	}
	else {
		blinnPhongShader->setUniform1i("material.hasOcclusion", 0);
	}
} 

void BlinnPhongMaterial::setDiffuseTexture(std::shared_ptr<TextureResource> diffuse) {
	this->diffuseTexture = diffuse;
}

void BlinnPhongMaterial::setSpecularTexture(std::shared_ptr<TextureResource> specular) {
	this->specularTexture = specular;
}

void BlinnPhongMaterial::setMetallicTexture(std::shared_ptr<TextureResource> metallic) {
	this->metallicTexture = metallic;
}

void BlinnPhongMaterial::setEmissiveTexture(std::shared_ptr<TextureResource> emissive) {
	this->emissiveTexture = emissive;
}

void BlinnPhongMaterial::setNormalMapTexture(std::shared_ptr<TextureResource> normalMap) {
	this->normalMapTexture = normalMap;
}

void BlinnPhongMaterial::setOcclusionTexture(std::shared_ptr<TextureResource> occlusion) {
	this->occlusionTexture = occlusion;
}

void BlinnPhongMaterial::setShininess(float shininess) {
	this->shininess = shininess;
}
