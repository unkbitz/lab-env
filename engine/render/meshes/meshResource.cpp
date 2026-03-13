#pragma once
#include "config.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "meshResource.h"

MeshResource::MeshResource() :vbo(0), ibo(0), vao(0), material(nullptr) {}

MeshResource::~MeshResource() {
	cleanUp();
}

void MeshResource::setUpBuffers() {
    if (vertices.empty() || indices.empty()) {
        std::cerr << "Error: No vertex or index data available to set up buffers." << std::endl;
        return;
    }
    std::cout << "Vertices size: " << vertices.size() << ", Indices size: " << indices.size() << std::endl;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

    bindBuffers();
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind
    glBindVertexArray(0); // Unbind
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind

    std::cout << "Buffer set up" << std::endl;
}

void MeshResource::bindBuffers() const {
	glEnableVertexAttribArray(0); // Position
	glEnableVertexAttribArray(1); // Normal
	glEnableVertexAttribArray(2); // Texture coordinates
	glEnableVertexAttribArray(3); // Tangents

	// Position (vec4)
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	// Normal (vec3)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	// Texture coordinates (vec2)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoord));
	// Tangents (vec4)
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tangent));

	std::cout << "Buffer bound" << std::endl;
}

void MeshResource::drawMesh() {
	//// Applying materials first
	//if (material) {
	//	material->Apply();
	//}
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void MeshResource::cleanUp() {
	if (vbo != 0) {
		glDeleteBuffers(1, &vbo);
		vbo = 0;
		std::cout << "vbo deleted" << endl;
	}
	if (ibo != 0) {
		glDeleteBuffers(1, &ibo);
		ibo = 0;
		std::cout << "ibo deleted" << endl;
	}
	if (vao != 0) {
		glDeleteVertexArrays(1, &vao);
		vao = 0;
		std::cout << "vao deleted" << endl;
	}
}

vec4 MeshResource::getPosition() {
	return transform.getPosition();
}

mat4 MeshResource::getRotation() {
	return transform.getRotation();
}

vec3 MeshResource::getScale() {
	return transform.getScale();
}

mat4 MeshResource::getTransform() {
	return transform.getTransformMatrix();
}

std::vector<Vertex> MeshResource::getVertices() {
	return vertices;
}

std::vector<int> MeshResource::getIndices() {
	return indices;
}

void MeshResource::setMaterial(std::shared_ptr<Material> mat) {
	this->material = mat;
}

void MeshResource::setPosition(vec4 const position) {
	transform.setPosition(position);
}

void MeshResource::setRotation(mat4 const rotation) {
	transform.setRotation(rotation);
}

void MeshResource::setScale(vec3 const scale) {
	transform.setScale(scale);
}

void MeshResource::setVertices(std::vector<Vertex> const vertices) {
	this->vertices = vertices;
}

void MeshResource::setIndices(std::vector<int> const indices) {
	this->indices = indices;
}

std::shared_ptr<MeshResource> MeshResource::createCube(float width, float height, float depth) {
	std::shared_ptr<MeshResource> mesh = std::make_shared<MeshResource>();

	height = height / 2;
	width = width / 2;
	depth = depth / 2;

	// Define positions for the cube's vertices
	vec4 cubePositions[] = {
		// Front face
		vec4(-width, -height,  depth, 1.0f), // Bottom-left
		vec4(width, -height,  depth, 1.0f), // Bottom-right
		vec4(width,  height,  depth, 1.0f), // Top-right
		vec4(-width,  height,  depth, 1.0f), // Top-left
		// Back face
		vec4(-width, -height, -depth, 1.0f), // Bottom-left
		vec4(width, -height, -depth, 1.0f), // Bottom-right
		vec4(width,  height, -depth, 1.0f), // Top-right
		vec4(-width,  height, -depth, 1.0f), // Top-left
	};

	// Define normals for the cube (pointing outwards)
	vec3 cubeNormals[] = {
		vec3(0.0f,  0.0f,  1.0f), // Front face
		vec3(0.0f,  0.0f, -1.0f), // Back face
		vec3(-1.0f,  0.0f,  0.0f), // Left face
		vec3(1.0f,  0.0f,  0.0f), // Right face
		vec3(0.0f,  1.0f,  0.0f), // Top face
		vec3(0.0f, -1.0f,  0.0f), // Bottom face
	};

	// Define texture coordinates
	vec2 texCoords[] = {
		vec2(1.0f, 1.0f), // Bottom-left
		vec2(0.0f, 1.0f), // Bottom-right
		vec2(0.0f, 0.0f), // Top-right
		vec2(1.0f, 0.0f), // Top-left
	};

	// Create the vertices for the cube
	mesh->vertices = {
		// Front face
		{ cubePositions[0], cubeNormals[0], texCoords[0] },
		{ cubePositions[1], cubeNormals[0], texCoords[1] },
		{ cubePositions[2], cubeNormals[0], texCoords[2] },
		{ cubePositions[3], cubeNormals[0], texCoords[3] },
		// Back face
		{ cubePositions[4], cubeNormals[1], texCoords[0] },
		{ cubePositions[5], cubeNormals[1], texCoords[1] },
		{ cubePositions[6], cubeNormals[1], texCoords[2] },
		{ cubePositions[7], cubeNormals[1], texCoords[3] },
		// Left face
		{ cubePositions[0], cubeNormals[2], texCoords[0] },
		{ cubePositions[3], cubeNormals[2], texCoords[1] },
		{ cubePositions[7], cubeNormals[2], texCoords[2] },
		{ cubePositions[4], cubeNormals[2], texCoords[3] },
		// Right face
		{ cubePositions[1], cubeNormals[3], texCoords[0] },
		{ cubePositions[5], cubeNormals[3], texCoords[1] },
		{ cubePositions[6], cubeNormals[3], texCoords[2] },
		{ cubePositions[2], cubeNormals[3], texCoords[3] },
		// Top face
		{ cubePositions[3], cubeNormals[4], texCoords[0] },
		{ cubePositions[2], cubeNormals[4], texCoords[1] },
		{ cubePositions[6], cubeNormals[4], texCoords[2] },
		{ cubePositions[7], cubeNormals[4], texCoords[3] },
		// Bottom face
		{ cubePositions[0], cubeNormals[5], texCoords[0] },
		{ cubePositions[1], cubeNormals[5], texCoords[1] },
		{ cubePositions[5], cubeNormals[5], texCoords[2] },
		{ cubePositions[4], cubeNormals[5], texCoords[3] },
	};

	// Define the indices for drawing the cube with triangles
	mesh->indices = {
		// Front face
		0, 1, 2, 2, 3, 0,
		// Back face
		6, 5, 4, 4, 7, 6,
		// Left face
		8, 9, 10, 10, 11, 8,
		// Right face
		12, 13, 14, 14, 15, 12,
		// Top face
		16, 17, 18, 18, 19, 16,
		// Bottom face
		22, 21, 20, 20, 23, 22,
	};

	std::cout << "Cube created" << std::endl;
	mesh->setUpBuffers();
	return mesh;
}


std::shared_ptr<MeshResource> MeshResource::loadFromOBJ(const std::string& filename)
{
	std::shared_ptr<MeshResource> mesh = std::make_shared<MeshResource>();

	std::vector<vec4> positions;
	std::vector<vec2> texCoords;
	std::vector<vec3> normals;

	// Face vectors
	std::vector<Vertex> vertices;
	std::vector<int> indices;
	std::unordered_map<std::tuple<int, int, int>, int, TupleHash> uniqueVertices;

	std::ifstream file(filename);
	std::string line;

	if (!file.is_open())
	{
		std::cerr << "Error opening OBJ file: " << filename << std::endl;
		return nullptr;
	}

	// Read the OBJ file line by line
	while (std::getline(file, line))
	{

		if (line.empty() || line[0] == '#')
		{
			continue;
		}

		std::stringstream ss(line);
		std::string prefix;
		ss >> prefix;

		if (prefix == "v")
		{
			vec4 tempPos;
			ss >> tempPos.x >> tempPos.y >> tempPos.z;
			tempPos.w = 1.0f;
			positions.push_back(tempPos);
		}
		else if (prefix == "vt")
		{
			vec2 tempTex;
			ss >> tempTex.x >> tempTex.y;
			texCoords.push_back(tempTex);
		}
		else if (prefix == "vn")
		{
			vec3 tempNorm;
			ss >> tempNorm.x >> tempNorm.y >> tempNorm.z;
			normals.push_back(tempNorm);
		}
		else if (prefix == "f")
		{
			std::vector<ObjIndex> face;
			std::string token;

			while (ss >> token)
			{
				ObjIndex idx;
				if (!parseFaceToken(token, idx))
				{
					std::cerr << "Failed to parse face token: " << token << std::endl;
					continue;
				}

				idx.v = fixIndex(idx.v, static_cast<int>(positions.size()));
				idx.vt = fixIndex(idx.vt, static_cast<int>(texCoords.size()));
				idx.vn = fixIndex(idx.vn, static_cast<int>(normals.size()));

				if (idx.v < 0 || idx.v >= static_cast<int>(positions.size()))
				{
					std::cerr << "Invalid position index in face: " << token << std::endl;
					continue;
				}

				face.push_back(idx);
			}
			if (face.size() < 3)
			{
				continue;
			}

			// make triangles of the sides
			// example: [0,1,2,3] -> (0,1,2), (0,2,3)
			for (size_t i = 1; i + 1 < face.size(); ++i)
			{
				ObjIndex tri[3] = { face[0], face[i], face[i + 1] };

				for (int j = 0; j < 3; ++j)
				{
					// create a unique key for the triangle
					const ObjIndex& idx = tri[j];
					std::tuple<int, int, int> key = std::make_tuple(idx.v, idx.vt, idx.vn);

					// check if the vertex already exists
					auto it = uniqueVertices.find(key);
					//if not:
					if (it == uniqueVertices.end())
					{
						// build vertex
						Vertex vertex{};
						vertex.position = positions[idx.v];

						//create texture corrdinates
						if (idx.vt >= 0 && idx.vt < static_cast<int>(texCoords.size()))
						{
							vertex.texCoord = texCoords[idx.vt];
						}

						else
						{
							vertex.texCoord = vec2(0.0f, 0.0f);
						}

						// create normals
						if (idx.vn >= 0 && idx.vn < static_cast<int>(normals.size()))
						{
							vertex.normal = normals[idx.vn];
							vertex.normal = normalize(vertex.normal);
						}
						else
						{
							vertex.normal = vec3(0.0f, 0.0f, 0.0f);
						}

						// store new vertex
						int newIndex = static_cast<int>(vertices.size());
						vertices.push_back(vertex);
						uniqueVertices[key] = newIndex;
						indices.push_back(newIndex);
					}
					else
					{
						indices.push_back(it->second);
					}
				}
			}
		}
	}
	file.close();

	// Check if positions are available (must have positions)
	if (positions.empty())
	{
		std::cerr << "Error: No position data in OBJ file." << std::endl;
		return nullptr;
	}

	// Print final sizes of the vertices and indices
	std::cout << "Final unique vertices size: " << vertices.size() << std::endl;
	std::cout << "Final indices size: " << indices.size() << std::endl;

	// Transfer the unique vertices and indices to the mesh
	mesh->vertices = std::move(vertices);
	mesh->indices = std::move(indices);

	mesh->setUpBuffers();
	return mesh;
}

int MeshResource::fixIndex(int idx, int size)
{
	if (idx > 0)
	{
		return idx - 1;
	}
	if (idx < 0)
	{
		return size + idx;
	}
	return -1;
}

bool MeshResource::parseFaceToken(const std::string& token, ObjIndex& out)
{
	out = ObjIndex{};
	size_t firstSlash = token.find('/');
	if (firstSlash == std::string::npos)
	{
		// v
		out.v = std::stoi(token);
		return true;
	}

	size_t secondSlash = token.find('/', firstSlash + 1);

	if (secondSlash == std::string::npos)
	{
		// v/vt
		std::string vStr = token.substr(0, firstSlash);
		std::string vtStr = token.substr(firstSlash + 1);

		if (!vStr.empty())
		{
			out.v = std::stoi(vStr);
		}
		if (!vtStr.empty())
		{
			out.vt = std::stoi(vtStr);
		}
		return true;
	}

	// v//vn or v/vt/vn
	std::string vStr = token.substr(0, firstSlash);
	std::string vtStr = token.substr(firstSlash + 1, secondSlash - firstSlash - 1);
	std::string vnStr = token.substr(secondSlash + 1);

	if (!vStr.empty())
	{
		out.v = std::stoi(vStr);
	}
	if (!vtStr.empty())
	{
		out.vt = std::stoi(vtStr);
	}
	if (!vnStr.empty())
	{
		out.vn = std::stoi(vnStr);
	}

	return true;
}
