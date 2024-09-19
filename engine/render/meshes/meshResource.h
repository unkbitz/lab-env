#pragma once
#include "render/Grid.h"
#include <unordered_map>
#include "math/mat4.h"

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
	void bindBuffers() const;
	void setUpBuffers();
	void drawMesh();
	vec3 getPosition() const;
	void setPosition(const vec3& position);
	static MeshResource createCube();
	void CleanUp();
};