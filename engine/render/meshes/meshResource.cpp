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
		// Front face
		-width,	-height,	depth,	/*bottom - left*/	0, 0, 0, 0,	/*color*/	0.0f, 0.0f,  /*Texture coordinates*/
		width,	-height,	depth,	/*bottom - right*/	0, 0, 0, 0,	/*color*/	1.0f, 0.0f,  /*Texture coordinates*/
		width,	height,		depth,	/*top - right*/		0, 0, 0, 0,	/*color*/	1.0f, 1.0f,  /*Texture coordinates*/
		-width,	height,		depth,	/*top - left*/		0, 0, 0, 0,	/*color*/	0.0f, 1.0f,  /*Texture coordinates*/

		// Back face
		-width,	-height,	-depth,	/*bottom-left*/		0, 0, 0, 0,	/*color*/	0.0f, 0.0f,  /*Texture coordinates*/
		width,	-height,	-depth,	/*bottom-right*/	0, 0, 0, 0,	/*color*/	1.0f, 0.0f,  /*Texture coordinates*/
		width,	height,		-depth,	/*top-right*/		0, 0, 0, 0,	/*color*/	1.0f, 1.0f,  /*Texture coordinates*/
		-width,	height,		-depth,	/*top-left*/		0, 0, 0, 0,	/*color*/	0.0f, 1.0f,  /*Texture coordinates*/

		// Left face
		-width,	-height,	-depth,	/*bottom-left*/		0, 0, 0, 0,	/*color*/	0.0f, 0.0f,  /*Texture coordinates*/
		-width,	-height,	depth,	/*bottom-right*/	0, 0, 0, 0,	/*color*/	1.0f, 0.0f,  /*Texture coordinates*/ 
		-width,	height,		depth,	/*top-right*/		0, 0, 0, 0,	/*color*/	1.0f, 1.0f,  /*Texture coordinates*/ 
		-width,	height,		-depth,	/*top-left*/		0, 0, 0, 0,	/*color*/	0.0f, 1.0f,  /*Texture coordinates*/ 

		// Right face
		width,	-height,	-depth,	/*bottom-left*/		0, 0, 0, 0,	/*color*/	0.0f, 0.0f,  /*Texture coordinates*/ 
		width,	-height,	depth,	/*bottom-right*/	0, 0, 0, 0,	/*color*/	1.0f, 0.0f,  /*Texture coordinates*/ 
		width,	height,		depth,	/*top-right*/		0, 0, 0, 0,	/*color*/	1.0f, 1.0f,  /*Texture coordinates*/ 
		width,	height,		-depth,	/*top-left*/		0, 0, 0, 0,	/*color*/	0.0f, 1.0f,  /*Texture coordinates*/ 

		// Top face
		-width,	height,		-depth,	/*top-left*/		0, 0, 0, 0,	/*color*/	0.0f, 0.0f,  /*Texture coordinates*/ 
		width,	height,		-depth,	/*top-right*/		0, 0, 0, 0,	/*color*/	1.0f, 0.0f,  /*Texture coordinates*/ 
		width,	height,		depth,	/*bottom-right*/	0, 0, 0, 0,	/*color*/	1.0f, 1.0f,  /*Texture coordinates*/ 
		-width,	height,		depth,	/*bottom-left*/		0, 0, 0, 0,	/*color*/	0.0f, 1.0f,  /*Texture coordinates*/ 

		// Bottom face
		-width,	-height,	-depth,	/*bottom-left*/		0, 0, 0, 0,	/*color*/	0.0f, 0.0f,  /*Texture coordinates*/ 
		width,	-height,	-depth,	/*bottom-right*/	0, 0, 0, 0,	/*color*/	1.0f, 0.0f,  /*Texture coordinates*/ 
		width,	-height,	depth,	/*top-right*/		0, 0, 0, 0,	/*color*/	1.0f, 1.0f,  /*Texture coordinates*/ 
		-width,	-height,	depth,	/*top-left*/		0, 0, 0, 0,	/*color*/	0.0f, 1.0f   /*Texture coordinates*/ 
		});

	mesh->indices.insert(mesh->indices.begin(), {
		// Front face
		0, 1, 2,
		2, 3, 0,

		// Back face
		4, 5, 6,
		6, 7, 4,

		// Left face
		8, 9, 10,
		10, 11, 8,

		// Right face
		12, 13, 14,
		14, 15, 12,

		// Top face
		16, 17, 18,
		18, 19, 16,

		// Bottom face
		20, 21, 22,
		22, 23, 20
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

vec3 MeshResource::getScale()
{
	return transform.getScale();
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
			std::cout << "Vertex Position: " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
		}
		else if (type == "vt") {
			// Texture coordinate
			vec2 tex;
			ss >> tex.x >> tex.y;
			texCoords.push_back(tex);
			std::cout << "Texture Coord: " << tex.x << ", " << tex.y << std::endl;
		}
		else if (type == "vn") {
			// Normal
			vec3 n;
			ss >> n.x >> n.y >> n.z;
			norm.push_back(n);
			std::cout << "Normal: " << n.x << ", " << n.y << ", " << n.z << std::endl;

		}
		else if (type == "f") {
			// Face (triangulated or quad)
			std::string vertex[4];  // In case of quads
			int count = 0;
			while (ss >> vertex[count++] && count < 4){}

			std::vector<int> vertexIndices;

			for (int i = 0; i < (count == 4 ? 4 : 3); i++) {
				std::istringstream vss(vertex[i]);
				std::string indices;
				int posIdx, texIdx = -1, normIdx = -1;

				std::getline(vss, indices, '/');
				posIdx = std::stoi(indices) - 1;

				if (std::getline(vss, indices, '/')) {
					texIdx = !indices.empty() ? std::stoi(indices) -1 : -1;
				}

				if (std::getline(vss, indices)) {
					normIdx = !indices.empty() ? std::stoi(indices) - 1 : -1;
				}
				std::cout << "Face vertex " << i << ": Position index: " << posIdx
					<< ", TexCoord index: " << texIdx
					<< ", Normal index: " << normIdx << std::endl;

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
				vertexIndices.push_back(mesh->vertices.size() / 9 - 1);
			}
			for (int idx : vertexIndices) {
				mesh->indices.push_back(idx);
			}

			// For quads, we split into two triangles
			if (count == 4) {
				
				// Add two triangles from quad face
				mesh->indices.push_back(vertexIndices[1]);
				mesh->indices.push_back(vertexIndices[2]);
				mesh->indices.push_back(vertexIndices[0]);

				mesh->indices.push_back(vertexIndices[2]);
				mesh->indices.push_back(vertexIndices[3]);
				mesh->indices.push_back(vertexIndices[0]);
			}
		}
	}

	file.close();
	std::cout << "Mesh created" << endl;
	mesh->setUpBuffers();
	return mesh;
}