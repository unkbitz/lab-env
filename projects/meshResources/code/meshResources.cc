#include "config.h"
#include "meshResources.h"
#include <cstring>
#include "math/mat4.h"
#include "Render/Grid.h"
#include "TextureResource.h"

const GLchar* vs =
"#version 430\n"
"layout(location=0) in vec3 pos;\n"
"layout(location=1) in vec4 color;\n"
"layout(location=2) in vec2 textureCoordinates;\n"
"layout(location=0) out vec4 Color;\n"
"uniform mat4 rotation;\n"
"uniform mat4 projection;\n"
"uniform mat4 view;\n"
"layout(location=2) out vec2 TextureCoordinates;\n"
"void main()\n"
"{\n"
"	gl_Position = projection * view * rotation * vec4(pos, 1);\n"
"	Color = color;\n"
"	TextureCoordinates = textureCoordinates;\n"
"}\n";

const GLchar* ps =
"#version 430\n"
"layout(location=2) in vec2 TextureCoordinates;\n"
"layout(location=0) in vec4 color;\n"
"out vec4 Color;\n"
"uniform sampler2D Texture;\n"
"void main()\n"
"{\n"
"	Color= texture(Texture, TextureCoordinates);\n"
"}\n";

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

			// setup vertex shader
			this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
			GLint length = static_cast<GLint>(std::strlen(vs));
			glShaderSource(this->vertexShader, 1, &vs, &length);
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
			length = static_cast<GLint>(std::strlen(ps));
			glShaderSource(this->pixelShader, 1, &ps, &length);
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
		float positionX = 0.0f;
		float positionY = 0.0f;
		float positionZ = 0.0f;
		float direction = 1.0f; // 1 for right, -1 for left
		float speed = 0.01f; // Movement speed
		float boundary = 0.5f; // Boundary for the movement (from -boundary to +boundary)

		texture::TextureResource texture("Capture.JPG");
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

			angle += 0.01;

			// do stuff
			
			glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

			mat4 rotationMatrix = rotationz(angle) * rotationx(angle) * rotationy(angle);
			vec4 meshPosition(sinf(angle), -0.5f, 0, 0);
			rotationMatrix[3] += meshPosition;
			mat4 transformMatrix = rotationMatrix;

			cameraPosition = vec3(cosf(angle) * 5 + meshPosition.x, 5.0f + meshPosition.y, sinf(angle) * 5 + meshPosition.z);
			cameraTarget[0] = meshPosition.x;
			cameraTarget[1] = meshPosition.y;
			cameraTarget[2] = meshPosition.z;
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