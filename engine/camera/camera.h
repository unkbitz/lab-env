#pragma once;
#include "math/mat4.h"

class camera {
private:
	vec3 cameraUp;
	vec3 cameraPosition;
	vec3 cameraTarget;
public:
	camera();
	~camera();
	
	vec3 getPosition() const;
	void setPosition(const vec3& position);
	vec3 getTarget() const;
	void setTarget(const vec3& target);
	vec3 getUp() const;
	void setUp(const vec3& up);
	mat4 getViewMatrix() const;
	mat4 getprojectionMatrix() const;

};