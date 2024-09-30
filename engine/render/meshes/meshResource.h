#pragma once
#include "render/Grid.h"
#include <unordered_map>
#include "math/mat4.h"
#include "meshTransform.h"

class MeshResource {
private:
	struct Vertex {
		vec4 position;
		vec3 normal;
		vec2 texCoord;
	};

	GLuint vbo;
	GLuint ibo;
	GLuint vao;
	std::vector<Vertex> vertices;
	std::vector<int> indices;
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
	void setVertices(std::vector<Vertex> const vertices);
	vec4 getPosition();
	mat4 getRotation();
	vec3 getScale();
};