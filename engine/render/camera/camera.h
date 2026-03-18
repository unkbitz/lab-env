#pragma once
#include "config.h"
#include "math/mat4.h"
#include "math.h"

class Camera {
public:
	Camera();
	~Camera();
	
	vec3 getPosition() const;
	void setPosition(const vec3& position);
	vec3 getTarget() const;
	void setTarget(const vec3& target);
	vec3 getUp() const;
	void setUp(const vec3& up);
	mat4 getViewMatrix() const;
	mat4 getProjectionMatrix(float aspect) const;
	mat4 getViewProjectionMatrix(float aspect) const;
	void updateCameraVectors();
	void rotate(float yawOffset, float pitchOffset);
	vec3 getFront() const;
	float getYaw() const;
	float getPitch() const;
	void setYaw(float newYaw);
	void setPitch(float newPitch);
	bool sphereInFrustum(const vec3& center, float radius, float aspect) const;

private:
	vec3 cameraUp;
	vec3 cameraPosition;
	vec3 cameraTarget;
	vec3 cameraFront;

	float yaw;
	float pitch;
};