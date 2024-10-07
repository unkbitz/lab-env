#pragma once
#include "config.h"
#include "lighting.h"

Lighting::Lighting() :
	pointLightPos(0.0f, 2.0f, 0.0f),
	pointLightColor(1.3f, 1.0f, 1.0f),
	pointLightIntensity(20.0f), 
	
	directionalLightDir(0.0f, -1.0f, 0.0f), 
	directionalLightColor(1.0f, 1.0f, 1.0f), 
	directionalLightIntensity(1.0f) {}

Lighting::~Lighting() {}

vec3 Lighting::getPointLightPos() const {
	return pointLightPos;
}

vec3 Lighting::getPointLightColor() const {
	return pointLightColor;
}

float Lighting::getPointLightIntensity() const {
	return pointLightIntensity;
}

vec3 Lighting::getDirectionalLightDir() const {
	return directionalLightDir;
}

vec3 Lighting::getDirectionalLightColor() const {
	return directionalLightColor;
}

float Lighting::getDirectionalLightIntensity() const {
	return directionalLightIntensity;
}

void Lighting::setPosition(const vec3& pos) {
	pointLightPos = pos;
}

void Lighting::setColor(const vec3& col) {
	pointLightColor = col;
}

void Lighting::setIntensity(const float& intens) {
	pointLightIntensity = intens;
}

void Lighting::setDirektionalLightDir(const vec3& dir) {
	directionalLightDir = dir;
}

void Lighting::setDirektionalLightColor(const vec3& col) {
	directionalLightColor = col;
}

void Lighting::setDirektionalLightIntensity(const float& intens) {
	directionalLightIntensity = intens;
}
