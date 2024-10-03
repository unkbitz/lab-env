#pragma once
#include "config.h"
#include "lighting.h"

Lighting::Lighting() :
	pointLightPos(0.0, 2.0, 0),
	color(1.0, 1.0, 1.0),
	intensity(20) {
}

Lighting::~Lighting() {}

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
