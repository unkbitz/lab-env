#include "config.h"
#include "meshResources.h"
#include <cstring>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iostream>
#include <string>
#include "math/mat4.h"
#include "Render/Grid.h"
#include "../engine/textures/TextureResource.h"

bool FileExists(const std::string& path) {
	std::ifstream file(path);
	return file.good();
}

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& path) {
	std::ifstream stream(path);
	if (!stream.is_open()) {
		std::cerr << "ERROR: Could not open shader file: " << path << std::endl;
		return { "", "" };
	}
	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}

using namespace Display;
namespace Mesh {
	MeshResources::MeshResources() {
		// empty
	}

	MeshResources::~MeshResources()	{
		CleanUp();
	}

	bool MeshResources::Open() {
		App::Open();
		this->window = new Display::Window;
		window->SetKeyPressFunction([this](int key, int scancode, int action, int mods)	{
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
				this->window->Close();
			}
		});

		GLfloat buf[] =	{
			-0.5f,	-0.5f,	0.5f,			// pos 0
			0.5,	0.5f,	0,		1,	// color 0
			0,		0,					//texture coordinates
			0.5f,	-0.5f,	0.5f,			// pos 1
			0.5f,	0,		0.5f,	1,	// color 0
			0,		1,					//texture coordinates
			0.5f,	0.5f,	0.5f,			// pos 2
			0,		0,		0.5f,	1,	// color 0
			1,		1,					//texture coordinates
			-0.5f,	0.5f,	0.5f,			// pos 2
			0,		1,		0,		1,	// color 0
			1,		0,					//texture coordinates

			-0.5f,	-0.5f,	-0.5f,			// pos 0
			0.5,	0.5f,	0,		1,	// color 0
			0,		0,					//texture coordinates
			0.5f,	-0.5f,	-0.5f,			// pos 1
			0.5f,	0,		0.5f,	1,	// color 0
			0,		1,					//texture coordinates
			0.5f,	0.5f,	-0.5f,			// pos 2
			0,		0,		0.5f,	1,	// color 0
			1,		1,					//texture coordinates
			-0.5f,	0.5f,	-0.5f,			// pos 2
			0,		1,		0,		1,	// color 0
			1,		0,					//texture coordinates
		};

		GLint iBuf[] = {
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
		};

		if (this->window->Open()) {
			// set clear color to gray
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

			std::string Shaderpath = "../engine/shaders/shader.txt";

			// setup vertex shader
			this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
			ShaderProgramSource source = ParseShader(Shaderpath);
			const char* vertexSource = source.VertexSource.c_str();
			GLint length = static_cast<GLint>(std::strlen(vertexSource));
			glShaderSource(this->vertexShader, 1, &vertexSource, &length);
			glCompileShader(this->vertexShader);

			// get error log
			GLint shaderLogSize;
			glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
			if (shaderLogSize > 0) {
				GLchar* buf = new GLchar[shaderLogSize];
				glGetShaderInfoLog(this->vertexShader, shaderLogSize, NULL, buf);
				printf("[SHADER COMPILE ERROR]: %s", buf);
				delete[] buf;
			}

			// setup pixel shader
			this->pixelShader = glCreateShader(GL_FRAGMENT_SHADER);
			const char* fragmentSource = source.FragmentSource.c_str();
			length = static_cast<GLint>(std::strlen(fragmentSource));
			
			glShaderSource(this->pixelShader, 1, &fragmentSource, &length);
			glCompileShader(this->pixelShader);

			// get error log
			shaderLogSize;
			glGetShaderiv(this->pixelShader, GL_INFO_LOG_LENGTH, &shaderLogSize);
			if (shaderLogSize > 0) {
				GLchar* buf = new GLchar[shaderLogSize];
				glGetShaderInfoLog(this->pixelShader, shaderLogSize, NULL, buf);
				printf("[SHADER COMPILE ERROR]: %s", buf);
				delete[] buf;
			}

			// create a program object
			this->program = glCreateProgram();
			glAttachShader(this->program, this->vertexShader);
			glAttachShader(this->program, this->pixelShader);
			glLinkProgram(this->program);
			glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &shaderLogSize);
			if (shaderLogSize > 0) {
				GLchar* buf = new GLchar[shaderLogSize];
				glGetProgramInfoLog(this->program, shaderLogSize, NULL, buf);
				printf("[PROGRAM LINK ERROR]: %s", buf);
				delete[] buf;
			}

			// setup vbo
			glGenBuffers(1, &this->triangle);
			glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
			glBufferData(GL_ARRAY_BUFFER, sizeof(buf), buf, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glGenBuffers(1, &ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(iBuf), iBuf, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			return true;
		}
		return false;
	}

	void MeshResources::Close() {
		if (this->window->IsOpen())
			this->window->Close();

		Core::App::Close();
	}

	void MeshResources::Run() {
		glEnable(GL_DEPTH_TEST);
		float angle = 0.0f;
		float speed = 0.01f; // Movement speed
		
		
		std::string texturePath = "../engine/textures/Capture.JPG";
		texture::TextureResource texture(texturePath);
		GLint textureLocation = glGetUniformLocation(this->program, "Texture");
		
		mat4 projectionMatrix = perspective(45.0f, 1.0f, 0.1f, 100.0f);
		vec3 cameraPosition(0.0f, 5.0f, 5.0f); //eye
		vec3 cameraTarget(0.0f, 0.0f, 0.0f); //at
		vec3 cameraUp(0.0f, 1.0f, 0.0f); //up
		mat4 viewMatrix = lookat(cameraPosition, cameraTarget, cameraUp);

		mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
		while (this->window->IsOpen()) {
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			this->window->Update();

			angle += 0.0005;

			// do stuff
			
			glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

			mat4 rotationMatrix;
			vec4 meshPosition(0.7, -0.5f, 0, 0);
			rotationMatrix[3] += meshPosition;
			mat4 transformMatrix = rotationMatrix;

			float radius = 5.0f;
			cameraPosition = vec3(cosf(angle) * radius + meshPosition.x, 5.0f, sinf(angle) * radius + meshPosition.z);
			cameraTarget = vec3(meshPosition.x, meshPosition.y, meshPosition.z);

			viewMatrix = lookat(cameraPosition, cameraTarget, cameraUp);
			viewProjectionMatrix = projectionMatrix * viewMatrix;

			glUseProgram(this->program);

			GLint rotationLocation = glGetUniformLocation(this->program, "rotation");
			if (rotationLocation != -1) {
				glUniformMatrix4fv(rotationLocation, 1, GL_FALSE, &transformMatrix[0][0]);
			}

			GLint projectionLocation = glGetUniformLocation(this->program, "projection");
			if (projectionLocation != -1) {
				glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
			}

			GLint viewLocation = glGetUniformLocation(this->program, "view");
			if (viewLocation != -1) {
				glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &viewMatrix[0][0]);
			}
			
			glUniform1i(textureLocation, 0);

			glEnableVertexAttribArray(0); //Position
			glEnableVertexAttribArray(1); //Color
			glEnableVertexAttribArray(2); //Texture coordinates
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 9, NULL);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 9, (GLvoid*)(sizeof(float32) * 3));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float32) * 9, (GLvoid*)(sizeof(float32) * 7));
			texture.bind();
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			Render::Grid grid;
			grid.Draw((GLfloat*) &viewProjectionMatrix[0][0]);
			this->window->SwapBuffers();

#ifdef CI_TEST
			// if we're running CI, we want to return and exit the application after one frame
			// break the loop and hopefully exit gracefully
			break;
#endif
		}
	}

	void MeshResources::CleanUp() {
		if (ibo != 0) {
			glDeleteBuffers(1,&ibo);
		}
		if (triangle != 0) {
			glDeleteBuffers(1, &triangle);
		}
	}

} // namespace Mesh