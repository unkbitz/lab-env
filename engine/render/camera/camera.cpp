#pragma once
#include "config.h"
#include "camera.h"

Camera::Camera():
	cameraUp(0.0f, 1.0f, 0.0f),
	cameraPosition(3.0f, 3.0f, 3.0f),
	cameraTarget(0.0f, 0.0f, 0.0f),
	yaw(-90.0f),
	pitch(0.0f),
	cameraFront(0.0f, 0.0f, -1.0f) {
	cameraFront = normalize(cameraTarget - cameraPosition);
	yaw = atan2(cameraFront.z, cameraFront.x) * 180.0f / 3.14159265358979323846f;
	pitch = asin(cameraFront.y) * 180.0f / 3.14159265358979323846f;
	updateCameraVectors();
}

Camera::~Camera() {}

vec3 Camera::getPosition() const
{
	return cameraPosition;
}

void Camera::setPosition(const vec3& position)
{
	cameraPosition = position;
}

vec3 Camera::getTarget() const
{
	return cameraTarget;
}

void Camera::setTarget(const vec3& target)
{
	cameraTarget = target;
}

vec3 Camera::getUp() const
{
	return cameraUp;
}

void Camera::setUp(const vec3& up)
{
	cameraUp = up;
}

mat4 Camera::getViewMatrix() const
{
	vec3 camP = cameraPosition;
	vec3 camF = cameraFront;
	return lookat(cameraPosition, camP + camF, cameraUp);
}

mat4 Camera::getProjectionMatrix(float aspect) const
{
	float fovRadians = 45.0f * (3.14159265358979323846f / 180.0f);
	return perspective(fovRadians, aspect, 0.1f, 100.0f);
}

mat4 Camera::getViewProjectionMatrix(float aspect) const 
{
	return this->getProjectionMatrix(aspect) * this->getViewMatrix();
}

void Camera::updateCameraVectors()
{
	vec3 front;
	float yawRadians = yaw * (3.14159265358979323846f / 180.0f);
	float pitchRadians = pitch * (3.14159265358979323846f / 180.0f);

	front.x = cosf(yawRadians) * cosf(pitchRadians);
	front.y = sinf(pitchRadians);
	front.z = sinf(yawRadians) * cosf(pitchRadians);

	cameraFront = normalize(front);
}

void Camera::rotate(float yawOffset, float pitchOffset)
{
	yaw += yawOffset;
	pitch += pitchOffset;

	//Avoid flipping the camera
	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	updateCameraVectors();
}

vec3 Camera::getFront() const
{
	return cameraFront;
}

float Camera::getYaw() const
{
	return this->yaw;
}

float Camera::getPitch() const
{
	return this->pitch;
}

void Camera::setYaw(float newYaw)
{
	yaw = newYaw;
}

void Camera::setPitch(float newPitch)
{
	pitch = newPitch;
}


bool Camera::sphereInFrustum(const vec3& center, float radius, float aspect) const
{
	mat4 vp = getViewProjectionMatrix(aspect);

	vec4 planes[6];

	//Left
	planes[0] = vec4(
		vp[0][3] + vp[0][0],
		vp[1][3] + vp[1][0],
		vp[2][3] + vp[2][0],
		vp[3][3] + vp[3][0]);
	//Right
	planes[1] = vec4(
		vp[0][3] - vp[0][0],
		vp[1][3] - vp[1][0],
		vp[2][3] - vp[2][0],
		vp[3][3] - vp[3][0]);
	//Bottom
	planes[2] = vec4(
		vp[0][3] + vp[0][1],
		vp[1][3] + vp[1][1],
		vp[2][3] + vp[2][1],
		vp[3][3] + vp[3][1]);
	//top
	planes[3] = vec4(
		vp[0][3] - vp[0][1],
		vp[1][3] - vp[1][1],
		vp[2][3] - vp[2][1],
		vp[3][3] - vp[3][1]);
	//Near
	planes[4] = vec4(
		vp[0][3] + vp[0][2],
		vp[1][3] + vp[1][2],
		vp[2][3] + vp[2][2],
		vp[3][3] + vp[3][2]);
	//Far
	planes[5] = vec4(
		vp[0][3] - vp[0][2],
		vp[1][3] - vp[1][2],
		vp[2][3] - vp[2][2],
		vp[3][3] - vp[3][2]);

	for (int i = 0; i < 6; i++)
	{
		vec3 normal(planes[i].x, planes[i].y, planes[i].z);
		float len = length(normal);
		if (len < 0.00001f)
		{
			continue;
		}

		normal.x = normal.x / len;
		normal.y = normal.y / len;
		normal.z = normal.z / len;
		float d = planes[i].w / len;

		float distance = dot(normal, center) + d;
		if (distance < -radius)
		{
			return false;
		}
	}

	return true;
}