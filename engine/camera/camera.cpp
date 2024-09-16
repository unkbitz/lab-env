#pragma once
#include "camera.h"

camera::camera():
	cameraUp(0.0f, 1.0f, 0.0f),
	cameraPosition(2.0f, 2.0f, 2.0f),
	cameraTarget(0.0f, 0.0f, 0.0f)
	{}
camera::~camera() {
	
}

vec3 camera::getPosition() const {
	return cameraPosition;
}

void camera::setPosition(const vec3& position) {
	cameraPosition = position;
}

vec3 camera::getTarget() const{
	return cameraTarget;
}

void camera::setTarget(const vec3& target) {
	cameraTarget = target;
}

vec3 camera::getUp() const {
	return cameraUp;
}

void camera::setUp(const vec3& up) {
	cameraUp = up;
}

mat4 camera::getViewMatrix() const {
	return lookat(cameraPosition, cameraTarget, cameraUp);
}

mat4 camera::getprojectionMatrix() const {
	return perspective(45.0f, 1.0f, 0.1f, 100.0f);
}