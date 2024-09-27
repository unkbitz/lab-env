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
	std::vector<float> vertices;
	std::vector<int> indices;
	std::vector<float> normals;
	MeshTransform transform;

public:
	MeshResource();
	~MeshResource();

	void bindBuffers() const;
	void setUpBuffers();
	void drawMesh();
	static std::shared_ptr<MeshResource>  createCube(float width, float height, float depth);
	static std::shared_ptr<MeshResource> loadFromOBJ(const std::string& filename);
	void cleanUp();
	mat4 getTransform();
	void setPosition(vec4 const position);
	void setRotation(mat4 const rotation);
	void setScale(vec3 const scale);
	vec4 getPosition();
	mat4 getRotation();
};