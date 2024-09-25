#pragma once
#include "config.h"
#include "meshTransform.h"


MeshTransform::MeshTransform() : meshScale(1, 1, 1), meshPos(0,0,0,1),
rotationMatrix(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, 1, 0), vec4(0, 0, 0, 1)),
transformMatrix(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, 1, 0), vec4(0, 0, 0, 1)) {

}

MeshTransform::~MeshTransform() {
}

vec4 MeshTransform::getPosition() const {
	return meshPos;
}
void MeshTransform::setPosition(const vec4& position) {
	meshPos = position;
}

mat4 MeshTransform::getRotation() const
{
	return rotationMatrix;
}

void MeshTransform::setRotation(const mat4& rotation)
{
	rotationMatrix = rotation;
}

vec3 MeshTransform::getScale() const
{
	return meshScale;
}

void MeshTransform::setScale(const vec3& scale)
{
	meshScale = scale;
}

mat4 MeshTransform::getTransformMatrix() const
{
	mat4 scaleMatrix = scaling(meshScale);
	mat4 translationMatrix = translation(meshPos.x, meshPos.y, meshPos.z);
	return translationMatrix * rotationMatrix * scaleMatrix;
}
