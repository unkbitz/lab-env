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
	mesh->vertices.insert(mesh->vertices.begin(),
		{
		-width,	-height,	depth,			// pos 0
		0.5,	0.5f,		0,		1,	// color 0
		1,		1,					//texture coordinates
		width,	-height,	depth,			// pos 1
		0.5f,	0,			0.5f,	1,	// color 0
		0,		1,					//texture coordinates
		width,	height,		depth,			// pos 2
		0,		0,			0.5f,	1,	// color 0
		0,		0,					//texture coordinates
		-width,	height,		depth,			// pos 2
		0,		1,			0,		1,	// color 0
		1,		0,					//texture coordinates

		-width,	-height,	-depth,			// pos 0
		0.5,	0.5f,		0,		1,	// color 0
		1,		1,					//texture coordinates
		width,	-height,	-depth,			// pos 1
		0.5f,	0,			0.5f,	1,	// color 0
		0,		1,					//texture coordinates
		width,	height,		-depth,			// pos 2
		0,		0,			0.5f,	1,	// color 0
		0,		0,					//texture coordinates
		-width,	height,		-depth,			// pos 2
		0,		1,			0,		1,	// color 0
		1,		0,					//texture coordinates
		});

	mesh->indices.insert(mesh->indices.begin(), {
		0, 1, 2,
		2, 3, 0,

		4, 6, 5,
		7, 6, 4,

		0, 4, 1,
		4, 5, 1,

		6, 7, 3,
		3, 2, 6,

		6, 5, 1,
		2, 6, 1,

		0, 3, 4,
		3, 7, 4
		});
	std::cout << "Cube created" << endl;
	mesh->setUpBuffers();
	return mesh;
}

void MeshResource::setUpBuffers() {
	if (vertices.empty() || indices.empty()) {
		std::cerr << "Error: No vertex or index data available to set up buffers." << std::endl;
		return; // Exit early to avoid exceptions
	}
	std::cout << "Vertices size: " << vertices.size() << ", Indices size: " << indices.size() << std::endl;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);


	size_t totalSize = (vertices.size() + normals.size()) * sizeof(float);
	glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());
	glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), normals.size() * sizeof(float), normals.data());


	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
	
	bindBuffers();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	std::cout << "Buffer set up" << endl;
}



void MeshResource::bindBuffers() const{
	glEnableVertexAttribArray(0); //Position
	glEnableVertexAttribArray(1); //Color
	glEnableVertexAttribArray(2); //Texture coordinates
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 9, NULL);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 9, (GLvoid*)(sizeof(float32) * 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float32) * 9, (GLvoid*)(sizeof(float32) * 7));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 11, (GLvoid*)(sizeof(float32) * 9));
	std::cout << "Buffer bound" << endl;
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

vec4 MeshResource::getPosition()
{
	return transform.getPosition();
}

mat4 MeshResource::getRotation()
{
	return transform.getRotation();
}

std::shared_ptr<MeshResource> MeshResource::loadFromOBJ(const std::string& filename)
{
	std::shared_ptr<MeshResource> mesh = std::make_shared<MeshResource>();
	std::ifstream file(filename);

	if (!file.is_open()) {
		std::cerr << "Error opening OBJ file: " << filename << std::endl;
		return nullptr;
	}

	std::vector<vec3> positions;
	std::vector<vec2> texCoords;
	std::vector<vec3> norm;

	std::string line;

	while (std::getline(file, line)) {
		std::istringstream ss(line);
		std::string type;
		ss >> type;

		if (type == "v") {
			// Vertex position
			vec3 pos;
			ss >> pos.x >> pos.y >> pos.z;
			positions.push_back(pos);
		}
		else if (type == "vt") {
			// Texture coordinate
			vec2 tex;
			ss >> tex.x >> tex.y;
			texCoords.push_back(tex);
		}
		else if (type == "vn") {
			// Normal
			vec3 n;
			ss >> n.x >> n.y >> n.z;
			norm.push_back(n);
		}
		else if (type == "f") {
			// Face (triangulated or quad)
			std::string vertex[4];  // In case of quads
			ss >> vertex[0] >> vertex[1] >> vertex[2] >> vertex[3];

			for (int i = 0; i < (vertex[3].empty() ? 3 : 4); i++) {
				std::istringstream vss(vertex[i]);
				std::string indices;
				int posIdx, texIdx, normIdx;

				std::getline(vss, indices, '/');
				posIdx = std::stoi(indices) - 1;  // OBJ index starts at 1

				std::getline(vss, indices, '/');
				texIdx = !indices.empty() ? std::stoi(indices) - 1 : -1;

				std::getline(vss, indices);
				normIdx = !indices.empty() ? std::stoi(indices) - 1 : -1;

				// Combine the vertices, texCoords, and normals
				mesh->vertices.push_back(positions[posIdx].x);
				mesh->vertices.push_back(positions[posIdx].y);
				mesh->vertices.push_back(positions[posIdx].z);

				if (texIdx != -1 && texIdx < texCoords.size()) {
					mesh->vertices.push_back(texCoords[texIdx].x);
					mesh->vertices.push_back(texCoords[texIdx].y);
				}
				else {
					mesh->vertices.push_back(0.0f); // Default values if no texture
					mesh->vertices.push_back(0.0f);
				}

				if (normIdx != -1 && normIdx < norm.size()) {
					mesh->normals.push_back(norm[normIdx].x);
					mesh->normals.push_back(norm[normIdx].y);
					mesh->normals.push_back(norm[normIdx].z);
				}
			}

			// For quads, we split into two triangles
			if (!vertex[3].empty()) {
				// Add two triangles from quad face
				mesh->indices.push_back(mesh->indices.size());
				mesh->indices.push_back(mesh->indices.size());
				mesh->indices.push_back(mesh->indices.size());

				mesh->indices.push_back(mesh->indices.size());
				mesh->indices.push_back(mesh->indices.size());
				mesh->indices.push_back(mesh->indices.size());
			}
			else {
				// Standard triangle
				mesh->indices.push_back(mesh->indices.size());
				mesh->indices.push_back(mesh->indices.size());
				mesh->indices.push_back(mesh->indices.size());
			}
		}
	}

	file.close();
	std::cout << "Mesh created" << endl;
	mesh->setUpBuffers();
	return mesh;
}