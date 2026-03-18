#pragma once
#include <unordered_map>
#include "math/mat4.h"
#include "render/gltf.h"
#include "meshTransform.h"
#include <tuple>
#include <functional>

struct TupleHash
{
	template <typename T>
	std::size_t operator()(T const& tuple) const
	{
		std::size_t h1 = std::hash<int>()(std::get<0>(tuple));
		std::size_t h2 = std::hash<int>()(std::get<1>(tuple));
		std::size_t h3 = std::hash<int>()(std::get<2>(tuple));

		// Combine the hash values of the individual elements
		return h1 ^ (h2 << 1) ^ (h3 << 2);
	}
};

struct ObjIndex
{
	int v = -1;
	int vt = -1;
	int vn = -1;
};

struct Vertex
{
	vec4 position;
	vec3 normal;
	vec2 texCoord;
	vec4 tangent;
};

class MeshResource
{
public:
	MeshResource();
	~MeshResource();

	void setUpBuffers();
	void bindBuffers() const;
	void drawMesh();
	void cleanUp();

	std::vector<Vertex> getVertices();
	std::vector<int> getIndices();

	void setVertices(std::vector<Vertex> const vertices);
	void setIndices(std::vector<int> const indices);

	static std::shared_ptr<MeshResource>  createCube(float width, float height, float depth);
	static std::shared_ptr<MeshResource> loadFromOBJ(const std::string& filename);

	static int fixIndex(int idx, int size);
	static bool parseFaceToken(const std::string& token, ObjIndex& out);

private:
	GLuint vbo;
	GLuint ibo;
	GLuint vao;
	std::vector<Vertex> vertices;
	std::vector<int> indices;
};