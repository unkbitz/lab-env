#pragma once
#include "render/Grid.h"
#include <unordered_map>
#include "math/mat4.h"
#include "meshTransform.h"

class MeshResource {
private:
	GLuint vbo;
	GLuint ibo;
	GLuint vao;

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
	static std::shared_ptr<MeshResource>  createCube(float width, float height, float depth);
	void cleanUp();
};