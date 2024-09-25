#pragma once
#include "render/Grid.h"
#include <unordered_map>
#include "math/mat4.h"

class MeshTransform {
public:
	MeshTransform();
	~MeshTransform();

	vec4 getPosition() const;
	mat4 getRotation() const;
	vec3 getScale() const;
	mat4 getTransformMatrix() const;
	void setPosition(const vec4& position);
	void setRotation(const mat4& rotation);
	void setScale(const vec3& scale);

private:
	vec3 meshScale;
	vec4 meshPos;
	mat4 rotationMatrix;
	mat4 translationMatrix;
	mat4 transformMatrix;
	mat4 scaleMatrix;

	void recalculate();
};