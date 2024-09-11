#include "config.h"
#include "meshResources.h"
#include <cstring>
#include "math/mat4.h"

const GLchar* vs =
"#version 430\n"
"layout(location=0) in vec3 pos;\n"
"layout(location=1) in vec4 color;\n"
"layout(location=0) out vec4 Color;\n"
"uniform mat4 rotation;\n"
"void main()\n"
"{\n"
"	gl_Position = rotation * vec4(pos, 1);\n"
"	Color = color;\n"
"}\n";

const GLchar* ps =
"#version 430\n"
"layout(location=0) in vec4 color;\n"
"out vec4 Color;\n"
"void main()\n"
"{\n"
"	Color = color;\n"
"}\n";

using namespace Display;
namespace Mesh
{
	MeshResources::MeshResources()
	{
		// empty
	}

	MeshResources::~MeshResources()
	{
		// empty
	}

	bool
	MeshResources::Open()
	{
		App::Open();
		this->window = new Display::Window;
		window->SetKeyPressFunction([this](int key, int scancode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
				this->window->Close();
			}
		});

		GLfloat buf[] =
		{
			-0.5f,	-0.5f,	0.5f,			// pos 0
			0.5,	0.5f,	0,		1,	// color 0
			0.5f,	-0.5f,	0.5f,			// pos 1
			0.5f,	0,		0.5f,	1,	// color 0
			0.5f,	0.5f,	0.5f,			// pos 2
			0,		0,		0.5f,	1,	// color 0
			-0.5f,	0.5f,	0.5f,			// pos 2
			0,		1,		0,		1,	// color 0

			-0.5f,	-0.5f,	-0.5f,			// pos 0
			0.5,	0.5f,	0,		1,	// color 0
			0.5f,	-0.5f,	-0.5f,			// pos 1
			0.5f,	0,		0.5f,	1,	// color 0
			0.5f,	0.5f,	-0.5f,			// pos 2
			0,		0,		0.5f,	1,	// color 0
			-0.5f,	0.5f,	-0.5f,			// pos 2
			0,		1,		0,		1,	// color 0
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

		if (this->window->Open())
		{
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
			if (shaderLogSize > 0)
			{
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
			if (shaderLogSize > 0)
			{
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
			if (shaderLogSize > 0)
			{
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

	void
	MeshResources::Close()
	{
		if (this->window->IsOpen())
			this->window->Close();

		Core::App::Close();
	}

	void
		MeshResources::Run()
	{
		glEnable(GL_DEPTH_TEST);
		float angle = 0.0f;
		float positionX = 0.0f;
		float direction = 1.0f; // 1 for right, -1 for left
		float speed = 0.01f; // Movement speed
		float boundary = 0.5f; // Boundary for the movement (from -boundary to +boundary)


		while (this->window->IsOpen())
		{
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			this->window->Update();

			angle += 0.01;
			positionX += speed * direction;
			if (positionX > boundary || positionX < -boundary) {
				direction *= -1.0f; // Change direction
			}

			// do stuff
			
			glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

			mat4 rotationMatrix = rotationz(angle)* rotationx(angle);
			rotationMatrix[3] += vec4(sin(angle), 0, 0, 0);
			mat4 transformMatrix = rotationMatrix;

			glUseProgram(this->program);
			GLint rotationLocation = glGetUniformLocation(this->program, "rotation");
			if (rotationLocation != -1) {
				glUniformMatrix4fv(rotationLocation, 1, GL_FALSE, &transformMatrix[0][0]);
			}

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, NULL);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float32) * 7, (GLvoid*)(sizeof(float32) * 3));
			
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			this->window->SwapBuffers();

#ifdef CI_TEST
			// if we're running CI, we want to return and exit the application after one frame
			// break the loop and hopefully exit gracefully
			break;
#endif
		}
	}

} // namespace Mesh