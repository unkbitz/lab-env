#pragma once
#include "render/Grid.h"
#include <unordered_map>
#include "math/mat4.h"
#include "meshTransform.h"
#include <tuple>
#include <functional>

struct TupleHash {
	template <typename T>
	std::size_t operator()(T const& tuple) const {
		std::size_t h1 = std::hash<int>()(std::get<0>(tuple));
		std::size_t h2 = std::hash<int>()(std::get<1>(tuple));
		std::size_t h3 = std::hash<int>()(std::get<2>(tuple));

		// Combine the hash values of the individual elements
		return h1 ^ (h2 << 1) ^ (h3 << 2);
	}
};

struct Vertex {
	vec4 position;
	vec3 normal;
	vec2 texCoord;
};

class MeshResource {
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
private:
	GLuint vbo;
	GLuint ibo;
	GLuint vao;
	std::vector<Vertex> vertices;
	std::vector<int> indices;
	MeshTransform transform;
};