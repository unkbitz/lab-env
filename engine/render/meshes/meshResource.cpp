#pragma once
#include "config.h"
#include "meshResource.h"

MeshResource::MeshResource() : 
	meshPos(0.0f, 0.0f, 0.0f), rotationMatrix(), transformMatrix() {
}

MeshResource::~MeshResource() {
	CleanUp();
}

vec3 MeshResource::getPosition() const {
	return meshPos;
}
void MeshResource::setPosition(const vec3& position) {
	meshPos = position;
}

MeshResource MeshResource::createCube() {
	MeshResource mesh;
	mesh.vertices.insert(mesh.vertices.begin(),
		{
		-0.5f,	-0.5f,	0.5f,			// pos 0
		0.5,	0.5f,	0,		1,	// color 0
		1,		1,					//texture coordinates
		0.5f,	-0.5f,	0.5f,			// pos 1
		0.5f,	0,		0.5f,	1,	// color 0
		0,		1,					//texture coordinates
		0.5f,	0.5f,	0.5f,			// pos 2
		0,		0,		0.5f,	1,	// color 0
		0,		0,					//texture coordinates
		-0.5f,	0.5f,	0.5f,			// pos 2
		0,		1,		0,		1,	// color 0
		1,		0,					//texture coordinates

		-0.5f,	-0.5f,	-0.5f,			// pos 0
		0.5,	0.5f,	0,		1,	// color 0
		1,		1,					//texture coordinates
		0.5f,	-0.5f,	-0.5f,			// pos 1
		0.5f,	0,		0.5f,	1,	// color 0
		0,		1,					//texture coordinates
		0.5f,	0.5f,	-0.5f,			// pos 2
		0,		0,		0.5f,	1,	// color 0
		0,		0,					//texture coordinates
		-0.5f,	0.5f,	-0.5f,			// pos 2
		0,		1,		0,		1,	// color 0
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
	return mesh;
}

void MeshResource::setUpBuffers() {
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MeshResource::bindBuffers() const{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glEnableVertexAttribArray(0); //Position
	glEnableVertexAttribArray(1); //Color
	glEnableVertexAttribArray(2); //Texture coordinates
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 9, NULL);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 9, (GLvoid*)(sizeof(float32) * 3));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float32) * 9, (GLvoid*)(sizeof(float32) * 7));
}

void MeshResource::drawMesh() {
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MeshResource::CleanUp() {
	//if (vbo != 0) {
	//	glDeleteBuffers(1, &vbo);
	//}
	//if (ibo != 0) {
	//	glDeleteBuffers(1, &ibo);
	//}
}