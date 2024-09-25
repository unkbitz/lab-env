#include "config.h"
#include "sarasExampleApp.h"
#include <fstream>
#include <sstream>
#include "math/mat4.h"
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



	if (this->window->Open())
	{
		//Creating the cube
		auto cubeMesh = MeshResource::createCube(1.0f, 1.0f, 1.0f);
		if (!cubeMesh) {
			std::cerr << "Failed to create cube mesh" << std::endl;
			return false;
		}
		cubeMesh->setUpBuffers();

		//Loading shader
		std::shared_ptr<ShaderResource> shader = std::make_shared<ShaderResource>();
		std::string Shaderpath = "assets/shader.txt";
		shader->load(Shaderpath);

		//Loading texture
		std::shared_ptr<TextureResource> texture = std::make_shared<TextureResource>();
		std::string texturePath = "assets/Capture.JPG";
		texture->load(texturePath);

		//Creating a GraphicsNode to manage the cube
		cubeNode = std::make_shared<GraphicsNode>();
		cubeNode->setMesh(cubeMesh);
		cubeNode->setShader(shader);
		cubeNode->setTexture(texture);
		//cubeNode->getTransform()->setScale(vec3(0.1f, 0.1f, 0.1f));
		cubeNode->getTransform()->setPosition(vec4(0.0, 0.0, -5.0, 1.0));
		grid = new Render::Grid();
		  
		// set clear color to gray
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		window->SetKeyPressFunction([this](int key, int scancode, int action, int mods) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
				this->window->Close();
			}
			if (action == GLFW_PRESS || action == GLFW_REPEAT) {
				switch (key) {
				case GLFW_KEY_W: cubePosition.z -= moveSpeed; break;
				case GLFW_KEY_S: cubePosition.z += moveSpeed; break;
				case GLFW_KEY_A: cubePosition.x -= moveSpeed; break;
				case GLFW_KEY_D: cubePosition.x += moveSpeed; break;
				}
			}
		});

		window->SetMouseMoveFunction([this](float xpos, float ypos) {
			if (mouseHeld) {
				float xoffset = xpos - lastMouseX;
				float yoffset = ypos - lastMouseY;

				lastMouseX = xpos;
				lastMouseY = ypos;

				float sensitivity = 0.01f;
				xoffset *= sensitivity;
				yoffset *= sensitivity;

				cubeRotation.y += xoffset;
				cubeRotation.x += yoffset;
			}
		});

		window->SetMousePressFunction([this](int button, int action, int mods) {
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
				mouseHeld = true;
			}
			else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
				mouseHeld = false;
			}
		});

		return true;
	}
	return false;
}
 
void ExampleApp::Close()
{
	if (this->window->IsOpen())
		this->window->Close();
	delete grid;

	Core::App::Close();
}

void ExampleApp::Run() {
	glEnable(GL_DEPTH_TEST);
	//float angle = 0.0f;
	//float speed = 0.0f; // Movement speed

	mat4 rotationMatrix;
	mat4 viewProjectionMatrix = cam.getprojectionMatrix() * cam.getViewMatrix();

	while (this->window->IsOpen()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->window->Update();

		// do stuff

		// Apply rotation
		rotationMatrix = rotationaxis(vec3(1, 0, 0), cubeRotation.x) * 
			rotationaxis(vec3(0, 1, 0), cubeRotation.y);

		cubeNode->getTransform()->setRotation(rotationMatrix);
		cubeNode->getTransform()->setPosition(cubePosition);

		cubeNode->getShader()->bind();
		viewProjectionMatrix = cam.getprojectionMatrix() * cam.getViewMatrix();
		cubeNode->draw(cam);
		grid->Draw((GLfloat*)&viewProjectionMatrix[0][0]);
		this->window->SwapBuffers();

#ifdef CI_TEST
		// if we're running CI, we want to return and exit the application after one frame
		// break the loop and hopefully exit gracefully
		break;
#endif
	}
}

} // namespace Example