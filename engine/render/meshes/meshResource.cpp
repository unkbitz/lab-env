#pragma once
#include "config.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "meshResource.h"

MeshResource::MeshResource() :vbo(0), ibo(0), vao(0) {
}

MeshResource::~MeshResource() {
	cleanUp();
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
		4, 5, 6, 6, 7, 4,
		// Left face
		8, 9, 10, 10, 11, 8,
		// Right face
		12, 13, 14, 14, 15, 12,
		// Top face
		16, 17, 18, 18, 19, 16,
		// Bottom face
		20, 21, 22, 22, 23, 20,
	};

	std::cout << "Cube created" << std::endl;

	mesh->setUpBuffers();
	return mesh;
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
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    std::cout << "Buffer set up" << std::endl;
}

void MeshResource::bindBuffers() const {
	glEnableVertexAttribArray(0); // Position
	glEnableVertexAttribArray(1); // Normal
	glEnableVertexAttribArray(2); // Texture coordinates

	// Position (vec4)
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	// Normal (vec3)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	// Texture coordinates (vec2)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoord));

	std::cout << "Buffer bound" << std::endl;
}


void MeshResource::drawMesh() {
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

mat4 MeshResource::getTransform() {
	return transform.getTransformMatrix();
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
	// Assign the input vertices to the internal vector
	this->vertices = vertices;
}

vec4 MeshResource::getPosition()
{
	return transform.getPosition();
}

mat4 MeshResource::getRotation()
{
	return transform.getRotation();
}

vec3 MeshResource::getScale()
{
	return transform.getScale();
}

std::shared_ptr<MeshResource> MeshResource::loadFromOBJ(const std::string& filename)
{
	std::shared_ptr<MeshResource> mesh = std::make_shared<MeshResource>();

	std::vector<vec4> positions;
	std::vector<vec2> texCoords;
	std::vector<vec3> normals;

	// Face vectors
	std::vector<GLint> positionIndices;
	std::vector<GLint> texCoordIndices;
	std::vector<GLint> normalIndices;

	std::ifstream file(filename);
	std::string line;
	std::string prefix;

	if (!file.is_open()) {
		std::cerr << "Error opening OBJ file: " << filename << std::endl;
		return nullptr;
	}

	// Read the OBJ file line by line
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		ss >> prefix;

		if (prefix == "v") {
			vec4 tempPos;
			ss >> tempPos.x >> tempPos.y >> tempPos.z;
			tempPos.w = 1.0f;
			positions.push_back(tempPos);
		}
		else if (prefix == "vt") {
			vec2 tempTex;
			ss >> tempTex.x >> tempTex.y;
			texCoords.push_back(tempTex);
		}
		else if (prefix == "vn") {
			vec3 tempNorm;
			ss >> tempNorm.x >> tempNorm.y >> tempNorm.z;
			normals.push_back(tempNorm);
		}
		else if (prefix == "f") {
			GLint posIdx, texIdx = -1, normIdx = -1;
			char slash; // to "remove" the slashes

			while (ss >> posIdx) {
				positionIndices.push_back(posIdx - 1);

				if (ss.peek() == '/') {
					ss >> slash; // "remove" first slash
					if (ss.peek() != '/') {
						ss >> texIdx;
						texCoordIndices.push_back(texIdx - 1);
					}

					if (ss.peek() == '/') {
						ss >> slash >> normIdx;
						normalIndices.push_back(normIdx - 1);
					}
				}
			}
		}
	}
	file.close();

	// Check if data has been correctly loaded
	std::cout << "Positions loaded: " << positions.size() << std::endl;
	std::cout << "Normals loaded: " << normals.size() << std::endl;
	std::cout << "Texture coordinates loaded: " << texCoords.size() << std::endl;
	std::cout << "Position indices: " << positionIndices.size() << std::endl;
	std::cout << "Normal indices: " << normalIndices.size() << std::endl;
	std::cout << "Texture coordinate indices: " << texCoordIndices.size() << std::endl;

	// Check if positions are available (must have positions)
	if (positions.empty()) {
		std::cerr << "Error: No position data in OBJ file." << std::endl;
		return nullptr;
	}

	// Construct the vertices from the loaded data
	std::vector<Vertex> vertices(positionIndices.size());

	for (size_t i = 0; i < positionIndices.size(); ++i) {
		vertices[i].position = positions[positionIndices[i]];

		// Check and assign texture coordinates, if available
		if (!texCoordIndices.empty()) {
			vertices[i].texCoord = texCoords[texCoordIndices[i]];
		}
		else {
			vertices[i].texCoord = vec2(0.0f, 0.0f); // Default texture coord if missing
		}

		// Check and assign normals, if available
		if (!normalIndices.empty()) {
			vertices[i].normal = normals[normalIndices[i]];
		}
		else {
			vertices[i].normal = vec3(0.0f, 0.0f, 0.0f); // Default normal if missing
		}
	}

	// Set the vertices into the MeshResource
	mesh->setVertices(vertices);

	// Make sure indices are created
	if (positionIndices.empty()) {
		std::cerr << "Error: No indices loaded from OBJ file." << std::endl;
		return nullptr;
	}

	// Set the index buffer to use the positionIndices as the element array
	mesh->indices = positionIndices;

	mesh->setUpBuffers();
	return mesh;
}
