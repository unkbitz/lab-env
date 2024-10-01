#pragma once
#include "config.h"
#include "camera.h"

Camera::Camera():
	cameraUp(0.0f, 1.0f, 0.0f),
	cameraPosition(4.0f, 4.0f, 4.0f),
	cameraTarget(0.0f, 0.0f, 0.0f),
	yaw(-90.0f),
	pitch(0.0f),
	cameraFront(0.0f, 0.0f, -1.0f){
	cameraFront = normalize(cameraTarget - cameraPosition);
	yaw = atan2(cameraFront.z, cameraFront.x) * 180.0f / 3.14159265358979323846f;
	pitch = asin(cameraFront.y) * 180.0f / 3.14159265358979323846f;
	updateCameraVectors();
}

Camera::~Camera() {
	
}

vec3 Camera::getPosition() const {
	return cameraPosition;
}

void Camera::setPosition(const vec3& position) {
	cameraPosition = position;
}

vec3 Camera::getTarget() const {
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

mat4 Camera::getViewMatrix()  {
	return lookat(cameraPosition, cameraPosition + cameraFront, cameraUp);
}

mat4 Camera::getProjectionMatrix() const {
	return perspective(45.0f, 1.0f, 0.1f, 100.0f);
}

mat4 Camera::getViewProjectionMatrix()  {
	return this->getProjectionMatrix() * this->getViewMatrix();
}

void Camera::updateCameraVectors() {
	vec3 front;
	float yawRadians = yaw * (3.14159265358979323846f / 180.0f);
	float pitchRadians = pitch * (3.14159265358979323846f / 180.0f);

	front.x = cosf(yawRadians) * cosf(pitchRadians);
	front.y = sinf(pitchRadians);
	front.z = sinf(yawRadians) * cosf(pitchRadians);

	// Debug outputs for trigonometric values
	std::cout << "cosYaw: " << cosf(yawRadians) << ", sinYaw: " << sinf(yawRadians) << std::endl;
	std::cout << "cosPitch: " << cosf(pitchRadians) << ", sinPitch: " << sinf(pitchRadians) << std::endl;

	cameraFront = normalize(front);
}
void Camera::rotate(float yawOffset, float pitchOffset) {
	yaw += yawOffset;
	pitch += pitchOffset;

	//Avoid flipping the camera
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	updateCameraVectors();
}

vec3 Camera::getFront() const {
	return cameraFront;
}

float Camera::getYaw() const {
	return this->yaw;
}

float Camera::getPitch() const {
	return this->pitch;
}

void Camera::setYaw(float newYaw) {
	yaw = newYaw;
}

void Camera::setPitch(float newPitch) {
	pitch = newPitch;
}
