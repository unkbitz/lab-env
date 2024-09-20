#pragma once
#include "config.h"
#include "camera.h"

Camera::Camera():
	cameraUp(0.0f, 1.0f, 0.0f),
	cameraPosition(4.0f, 4.0f, 4.0f),
	cameraTarget(0.0f, 0.0f, 0.0f)
	{}
Camera::~Camera() {
	
}

vec3 Camera::getPosition() const {
	return cameraPosition;
}

void Camera::setPosition(const vec3& position) {
	cameraPosition = position;
}

vec3 Camera::getTarget() const{
	return cameraTarget;
}

void Camera::setTarget(const vec3& target) {
	cameraTarget = target;
}

vec3 Camera::getUp() const {
	return cameraUp;
}

void Camera::setUp(const vec3& up) {
	cameraUp = up;
}

mat4 Camera::getViewMatrix() const {
	return lookat(cameraPosition, cameraTarget, cameraUp);
}

mat4 Camera::getprojectionMatrix() const {
	return perspective(45.0f, 1.0f, 0.1f, 100.0f);
}