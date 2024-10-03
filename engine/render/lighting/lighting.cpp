#pragma once
#include "config.h"
#include "lighting.h"

Lighting::Lighting() :
	pointLightPos(0.0f, 2.0f, 0.0f),
	pointLightColor(1.3f, 1.0f, 1.0f),
	pointLightIntensity(25.0f), 
	
	direktionalLightDir(0.0f, -1.0f, 0.0f), 
	direktionalLightColor(1.0f, 1.0f, 1.0f), 
	direktionalLightIntensity(0.2f) {
}

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

vec3 Lighting::getDirektionalLightDir() const {
	return direktionalLightDir;
}

vec3 Lighting::getDirektionalLightColor() const {
	return direktionalLightColor;
}

float Lighting::getDirektionalLightIntensity() const {
	return direktionalLightIntensity;
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
	direktionalLightDir = dir;
}

void Lighting::setDirektionalLightColor(const vec3& col) {
	direktionalLightColor = col;
}

void Lighting::setDirektionalLightIntensity(const float& intens) {
	direktionalLightIntensity = intens;
}
