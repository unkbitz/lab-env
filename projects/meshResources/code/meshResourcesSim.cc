#include "config.h"
#include "meshResourcesSim.h"
#include <fstream>
#include <sstream>
#include "math/mat4.h"
#include "Render/Grid.h"
#include "render/camera/camera.cpp"
#include "render/textures/textureResource.h"

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

			// setup shader
			std::string Shaderpath = "assets/shader.txt";
			shader.Load(Shaderpath);
			shader.Bind();

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
		bool direction = true;
		camera cam;
		
		std::string texturePath = "assets/Capture.JPG";
		texture::TextureResource texture(texturePath);
		const std::string Texture = "Texture";
		GLint textureLocation = shader.GetUniformLocation(Texture);

		vec4 meshPos(0.5f, 0.5f, 0.0f, 0); 
		mat4 viewMatrix = cam.getViewMatrix();
		mat4 projectionMatrix = cam.getprojectionMatrix();
		
		mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
		while (this->window->IsOpen()) {
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			this->window->Update();
			glUseProgram(shader.getProgram());

			// do stuff
			glBindBuffer(GL_ARRAY_BUFFER, this->triangle);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

			float radius = 5.0f;
			angle += 0.005;
			mat4 rotationMatrix = rotationaxis(vec3(0, 1, 0), angle);
			if (direction) {
				meshPos.x += 0.01f;
				if (meshPos.x >= 1) {
					direction = false;
				}
			}
			if (!direction) {
				meshPos.x -= 0.01f;
				if (meshPos.x <= -1) {
					direction = true;
				}
			}
			rotationMatrix[3] += meshPos;
			mat4 transformMatrix = rotationMatrix;

			vec3 target(meshPos.x, meshPos.y, meshPos.z);
			cam.setTarget(target);
			cam.setPosition(vec3(
				(cosf(angle) * radius + meshPos.x), 
				(5.0f + meshPos.y), 
				(sinf(angle) * radius + meshPos.z)));

			viewMatrix = cam.getViewMatrix();
			viewProjectionMatrix = projectionMatrix * viewMatrix;
			
			shader.setUniformMat4("rotation", transformMatrix, shader.getProgram());
			shader.setUniformMat4("projection", projectionMatrix, shader.getProgram());
			shader.setUniformMat4("view", viewMatrix, shader.getProgram());
			
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