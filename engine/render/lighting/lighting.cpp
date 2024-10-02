#pragma once
#include "config.h"
#include "lighting.h"

Lighting::Lighting() :
	pointLightPos(0.0, 10.0, 0),
	color(1.0, 1.0, 1.0),
	intensity(1) {
}

Lighting::~Lighting() {
}

void Lighting::addToShader(const std::shared_ptr<ShaderResource>& shader) {
	shader->setUniform3f("u_LightPos", pointLightPos.x, pointLightPos.y, pointLightPos.z);
	shader->setUniform3f("u_LightColor", color.x, color.y, color.z);
	shader->setUniform1f("u_LightIntensity", intensity);
}

vec3 Lighting::getPosition() const {
	return pointLightPos;
}

vec3 Lighting::getColor() const {
	return color;
}

float Lighting::getIntesity() const {
	return intensity;
}

void Lighting::setPosition(const vec3& pos) {
	pointLightPos = pos;
}

void Lighting::setColor(const vec3& col) {
	color = col;
}

void Lighting::setIntensity(const float& intens) {
	intensity = intens;
}
