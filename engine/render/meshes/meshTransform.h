#pragma once
#include "render/Grid.h"
#include <unordered_map>
#include "math/mat4.h"

class MeshTransform {
private:
	vec3 meshScale;
	vec4 meshPos;
	mat4 rotationMatrix;
	mat4 transformMatrix;
public:
	MeshTransform();
	~MeshTransform();

	vec4 getPosition() const;
	void setPosition(const vec4& position);
	mat4 getRotation() const;
	void setRotation(const mat4& rotation);
	vec3 getScale() const;
	void setScale(const vec3& scale);
	mat4 getTransformMatrix() const;
};