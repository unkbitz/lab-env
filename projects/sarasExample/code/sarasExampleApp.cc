#include "config.h"
#include "sarasExampleApp.h"
#include <fstream>
#include <sstream>
#include "math/mat4.h"
#include "Render/Grid.h"
using namespace Display;
namespace Example
{
ExampleApp::ExampleApp()
{
	// empty
}
ExampleApp::~ExampleApp()
{
	// empty
}

bool ExampleApp::Open()
{
	App::Open();
	this->window = new Display::Window;
	window->SetKeyPressFunction([this](int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			this->window->Close();
		}
	});

	cube = MeshResource::createCube(1.0, 1.0, 1.0);
	//cube2 = MeshResource::createCube();
	if (this->window->Open())
	{
		// set clear color to gray
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		// setup shader
		std::string Shaderpath = "assets/shader.txt";
		shader.Load(Shaderpath);
		shader.Bind();

		cube.setUpBuffers();
		return true;
	}
	return false;
}

void ExampleApp::Close()
{
	if (this->window->IsOpen())
		this->window->Close();

	Core::App::Close();
}

void ExampleApp::Run() {
	glEnable(GL_DEPTH_TEST);
	float angle = 0.0f;
	float speed = 0.01f; // Movement speed
	bool direction = true;
	vec3 target;
	vec4 meshPos(0.0f, 0.5f, 0.0f, 0.0f);
	mat4 rotationMatrix;
	
	std::string texturePath = "assets/Capture.JPG";
	texture.load(texturePath);
	const std::string Texture = "Texture";
	GLint textureLocation = shader.GetUniformLocation(Texture);

	cube.transform.setPosition(meshPos);
	mat4 viewProjectionMatrix = cam.getprojectionMatrix() * cam.getViewMatrix();
	while (this->window->IsOpen()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->window->Update();
		glUseProgram(shader.getProgram());

		// do stuff
		float radius = 5.0f;
		angle += 0.005;
		cube.transform.setRotation(rotationaxis(vec3(0, 1, 0), angle));
		rotationMatrix = cube.transform.getRotation();
		if (direction) {
			cube.transform.setPosition(vec4(cube.transform.getPosition().x + speed, 0.5f, 0.0f, 0.0f));
			if (cube.transform.getPosition().x >= 1) {
				direction = false;
			}
		}
		if (!direction) {
			cube.transform.setPosition(vec4(cube.transform.getPosition().x - speed, 0.5f, 0.0f, 0.0f));
			if (cube.transform.getPosition().x <= -1) {
				direction = true;
			}
		}
		rotationMatrix[3] += cube.transform.getPosition();
		cube.transform.setTransformMatrix(rotationMatrix);
		target.x = cube.transform.getPosition().x;
		target.y = cube.transform.getPosition().y;
		target.z = cube.transform.getPosition().z;

		cam.setTarget(target);
		cam.setPosition(vec3(
			(cosf(angle) * radius + cube.transform.getPosition().x),
			(5.0f + cube.transform.getPosition().y),
			(sinf(angle) * radius + cube.transform.getPosition().z)));

		viewProjectionMatrix = cam.getprojectionMatrix() * cam.getViewMatrix();

		shader.setUniformMat4("rotation", cube.transform.getTransformMatrix(), shader.getProgram());
		shader.setUniformMat4("projection", cam.getprojectionMatrix(), shader.getProgram());
		shader.setUniformMat4("view", cam.getViewMatrix(), shader.getProgram());

		glUniform1i(textureLocation, 0);
		cube.bindBuffers();
		texture.bind();
		cube.drawMesh();
		Render::Grid grid;
		grid.Draw((GLfloat*)&viewProjectionMatrix[0][0]);
		this->window->SwapBuffers();

#ifdef CI_TEST
		// if we're running CI, we want to return and exit the application after one frame
		// break the loop and hopefully exit gracefully
		break;
#endif
	}
}

} // namespace Example