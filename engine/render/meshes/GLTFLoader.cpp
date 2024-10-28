#pragma once
#include "config.h"
#include "GLTFLoader.h"

GLTFLoader::GLTFLoader() {}

GLTFLoader::~GLTFLoader() {}

std::shared_ptr<GraphicsNode> GLTFLoader::loadGLTFRootNode(const std::string& uri, int imageFlip, string folderPath) {
	fx::gltf::Document document = fx::gltf::LoadFromText(uri);

	// Create a root node for the entire model
	auto rootNode = std::make_shared<GraphicsNode>();
	// Iterate through top-level nodes in the document
	for (size_t i = 0; i < document.scenes.at(0).nodes.size(); ++i) {
		auto nextNode = loadGLTFNode(document, document.scenes.at(0).nodes[i], imageFlip, folderPath);
		if (i == 0) {
			rootNode = nextNode;
		}
		else {
			rootNode->addChild(nextNode);
		}
	}
	return rootNode;
}

std::shared_ptr<GraphicsNode> GLTFLoader::loadGLTFNode(const fx::gltf::Document& document, int nodeIndex, int imageFlip, string folderPath) {
	const auto& gltfNode = document.nodes.at(nodeIndex);
	auto graphicsNode = std::make_shared<GraphicsNode>();
	// If the node has a mesh, load it
	if (gltfNode.mesh >= 0) {
		const auto& mesh = document.meshes.at(gltfNode.mesh);
		auto meshResource = std::make_shared<MeshResource>();

		// Iterate through the primitives of the mesh
		for (const auto& primitive : mesh.primitives) {
			// Creating accessors for positions, normals, and texture coordinates
			const auto& positionAccessor = document.accessors.at(primitive.attributes.at("POSITION"));
			const auto& normalAccessor = document.accessors.at(primitive.attributes.at("NORMAL"));
			const auto& texCoordAccessor = document.accessors.at(primitive.attributes.at("TEXCOORD_0"));

			// Loading position data
			const auto& positionBufferView = document.bufferViews.at(positionAccessor.bufferView);
			const auto& positionBuffer = document.buffers.at(positionBufferView.buffer);
			const float* positions = reinterpret_cast<const float*>(&positionBuffer.data.at(positionBufferView.byteOffset));

			// Loading normal data
			const auto& normalBufferView = document.bufferViews.at(normalAccessor.bufferView);
			const auto& normalBuffer = document.buffers.at(normalBufferView.buffer);
			const float* normals = reinterpret_cast<const float*>(&normalBuffer.data.at(normalBufferView.byteOffset));

			// Loading texture coordinates
			const auto& texCoordBufferView = document.bufferViews.at(texCoordAccessor.bufferView);
			const auto& texCoordBuffer = document.buffers.at(texCoordBufferView.buffer);
			const float* texCoords = reinterpret_cast<const float*>(&texCoordBuffer.data.at(texCoordBufferView.byteOffset));

			// Creating vertices
			std::vector<Vertex> vertices;
			for (size_t i = 0; i < positionAccessor.count; ++i) {
				Vertex vertex;
				vertex.position = vec4(positions[i * 3], positions[i * 3 + 1], positions[i * 3 + 2], 1.0f);
				vertex.normal = vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]);
				vertex.texCoord = vec2(texCoords[i * 2], texCoords[i * 2 + 1]);
				vertices.push_back(vertex);
			}

			// Set vertices in mesh resource
			meshResource->setVertices(vertices);

			// Handle indices
			if (primitive.indices >= 0) {
				const auto& indexAccessor = document.accessors.at(primitive.indices);
				const auto& indexBufferView = document.bufferViews.at(indexAccessor.bufferView);
				const auto& indexBuffer = document.buffers.at(indexBufferView.buffer);
				const uint16_t* indices = reinterpret_cast<const uint16_t*>(&indexBuffer.data.at(indexBufferView.byteOffset));

				std::vector<int> elementIndices(indices, indices + indexAccessor.count);
				meshResource->setIndices(elementIndices);
			}

			// Setting shader
			std::shared_ptr<ShaderResource>shader = std::make_shared<ShaderResource>();
			std::string shaderPath = "assets/blinn_phong.shader";
			shader->load(shaderPath);
			graphicsNode->setShader(shader);

			// Handle material loading
			std::shared_ptr<BlinnPhongMaterial> material = std::make_shared<BlinnPhongMaterial>(shader);
			if (primitive.material >= 0) {
				const auto& materialInfo = document.materials.at(primitive.material);

				material->setShininess(32.0f);

				// Load diffuse texture
				if (materialInfo.pbrMetallicRoughness.baseColorTexture.index >= 0) {
					auto diffuseTexture = loadTexture(document, materialInfo.pbrMetallicRoughness.baseColorTexture.index, imageFlip, folderPath);
					material->setDiffuseTexture(diffuseTexture);
				}

				// Load specular texture if available
				if (materialInfo.pbrMetallicRoughness.metallicRoughnessTexture.index >= 0) {
					auto specularTexture = loadTexture(document, materialInfo.pbrMetallicRoughness.metallicRoughnessTexture.index, imageFlip, folderPath);
					material->setSpecularTexture(specularTexture);
				}

				// Load emissive texture if available
				if (materialInfo.emissiveTexture.index >= 0) {
					auto emissiveTexture = loadTexture(document, materialInfo.emissiveTexture.index, imageFlip, folderPath);
					material->setEmissiveTexture(emissiveTexture);
				}
			}

			// Assign the mesh resource to the graphics node
			graphicsNode->setMesh(meshResource);
			graphicsNode->setMaterial(material);
			meshResource->setUpBuffers();
		}
	}
	return graphicsNode;
}

std::shared_ptr<TextureResource> GLTFLoader::loadTexture(const fx::gltf::Document& document, int textureIndex, int imageFlip, string folderPath) {
	if (textureIndex < 0 || textureIndex >= document.textures.size()) {
		return nullptr; // Invalid texture index
	}
	const auto& texture = document.textures[textureIndex];
	const auto& image = document.images[texture.source];

	std::shared_ptr<TextureResource> textureResource = std::make_shared<TextureResource>();
	textureResource->loadTextureURI(folderPath + image.uri, imageFlip);
	// Load texture data (URI handling and stb_image loading)
	if (!textureResource) {
		std::cout << "failed to load texture" << std::endl;
		return nullptr;
	}

	return textureResource;
}
