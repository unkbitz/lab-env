#pragma once
#include "config.h"
#include "meshResource.h"

MeshResource::MeshResource() :vbo(0), ibo(0), vao(0) {
}

MeshResource::~MeshResource() {
	cleanUp();
}

MeshResource MeshResource::createCube(float width, float height, float depth) {
	MeshResource mesh;
	height = height / 2;
	width = width / 2;
	depth = depth / 2;
	mesh.vertices.insert(mesh.vertices.begin(),
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

	mesh.indices.insert(mesh.indices.begin(), {
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
	return mesh;
}

void MeshResource::setUpBuffers() {
	if (vertices.empty() || indices.empty()) {
		std::cerr << "Error: No vertex or index data available to set up buffers." << std::endl;
		return; // Exit early to avoid exceptions
	}
	std::cout << "Vertices size: " << vertices.size() << ", Indices size: " << indices.size() << std::endl;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	

	std::cout << "Buffer set up" << endl;
}

void MeshResource::bindBuffers() const{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0); //Position
	glEnableVertexAttribArray(1); //Color
	glEnableVertexAttribArray(2); //Texture coordinates
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 9, NULL);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 9, (GLvoid*)(sizeof(float32) * 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float32) * 9, (GLvoid*)(sizeof(float32) * 7));
	std::cout << "Buffer bound" << endl;
}

void MeshResource::drawMesh() {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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