#pragma once
#include "render/Grid.h"
#include <unordered_map>
#include "math/mat4.h"
#include "meshTransform.h"

class MeshResource {
private:
	GLuint vbo = 0;
	GLuint ibo = 0;
	GLuint vao = 0;

	vec3 meshPos;
	mat4 rotationMatrix;
	mat4 transformMatrix;
	std::vector<float> vertices;
	std::vector<int> indices;
public:
	MeshResource();
	~MeshResource();

	MeshTransform transform;

	void bindBuffers() const;
	void setUpBuffers();
	void drawMesh();
	static MeshResource createCube(float width, float height, float depth);
	void cleanUp();
};