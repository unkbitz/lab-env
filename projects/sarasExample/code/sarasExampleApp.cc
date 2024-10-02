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


	if (this->window->Open())
	{
		//Creating light
		std::shared_ptr<Lighting> light = std::make_shared<Lighting>();

		//Creating a mesh by loading OBJ
		std::shared_ptr<MeshResource> meshTest = MeshResource::loadFromOBJ("assets/KUBEN.obj");//"assets/lowpolydeer/deer.obj"
		if (!meshTest) { 
			std::cerr << "Failed to load OBJ mesh" << std::endl;
			return false;
		}
		
		//Creating the cube
		std::shared_ptr<MeshResource> cubeMesh = MeshResource::createCube(0.5f, 0.5f, 0.5f);
		if (!cubeMesh) {
			std::cerr << "Failed to create cube mesh" << std::endl;
			return false;
		}
		
		//Loading shader
		std::shared_ptr<ShaderResource> shader = std::make_shared<ShaderResource>();
		std::shared_ptr<ShaderResource> blinnPhongShader = std::make_shared<ShaderResource>();
		std::string lightShaderPath = "assets/blinn_phong.txt";
		std::string shaderPath = "assets/shader.txt";
		shader->load(shaderPath);
		blinnPhongShader->load(lightShaderPath);

		//Loading texture
		std::shared_ptr<TextureResource> texture = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> rubikTex = std::make_shared<TextureResource>();
		rubikTex->load("assets/Rubik2.png");
		texture->load("assets/Capture.JPG");

		//Creating a GraphicsNodes
		cubeNode = std::make_shared<GraphicsNode>();
		meshTestNode = std::make_shared<GraphicsNode>();
		lightNode = std::make_shared<GraphicsNode>();
		//Set Nodes meshes, shaders and textures
		cubeNode->setMesh(cubeMesh);
		cubeNode->setShader(shader);
		cubeNode->setTexture(rubikTex);
		meshTestNode->setMesh(meshTest);
		meshTestNode->setShader(shader);
		meshTestNode->setTexture(texture);
		lightNode->setShader(blinnPhongShader);

		mat4 rotationMatrix;
		cubeNode->setRotation(rotationMatrix);
		cubeNode->setPosition(vec4(0.0, 0.25, 0.0, 1.0));
		meshTestNode->setRotation(rotationMatrix);
		meshTestNode->setPosition(vec4(1.0, 0.25, 1.0, 1.0));

		
		
		grid = new Render::Grid();
		  
		// set clear color to gray
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		window->SetKeyPressFunction([this](int key, int scancode, int action, int mods) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
				this->window->Close();
			}
			vec4 cubeMovement(0, 0, 0, 0);
			vec4 meshMovement(0, 0, 0, 0);
			vec3 camMovement(0, 0, 0);
			if (mouseRightHeld) {
				if (action == GLFW_PRESS || action == GLFW_REPEAT) {
					switch (key) {
					case GLFW_KEY_W: camMovement += cam.getFront() * moveSpeed; break;
					case GLFW_KEY_S: camMovement -= cam.getFront() * moveSpeed; break;
					case GLFW_KEY_A: camMovement -= normalize(cross(cam.getFront(), cam.getUp())) * moveSpeed; break;
					case GLFW_KEY_D: camMovement += normalize(cross(cam.getFront(), cam.getUp())) * moveSpeed; break;
					}
				}
			}
			else if (!mouseRightHeld) {
				if (action == GLFW_PRESS || action == GLFW_REPEAT) {
					switch (key) {
					case GLFW_KEY_W: cubeMovement.z -= moveSpeed; break;
					case GLFW_KEY_S: cubeMovement.z += moveSpeed; break;
					case GLFW_KEY_A: cubeMovement.x -= moveSpeed; break;
					case GLFW_KEY_D: cubeMovement.x += moveSpeed; break;
					}
				}
				if (action == GLFW_PRESS || action == GLFW_REPEAT) {
					switch (key) {
					case GLFW_KEY_UP: meshMovement.z -= moveSpeed; break;
					case GLFW_KEY_DOWN: meshMovement.z += moveSpeed; break;
					case GLFW_KEY_LEFT: meshMovement.x -= moveSpeed; break;
					case GLFW_KEY_RIGHT: meshMovement.x += moveSpeed; break;
					}
				}
			}
			cam.setPosition(cam.getPosition() + camMovement);
			cam.updateCameraVectors();
			meshTestNode->setPosition(meshTestNode->getPosition() + meshMovement);
			cubeNode->setPosition(cubeNode->getPosition() + cubeMovement);
		});

		vec3 cubeRotation(0, 0, 0);
		window->SetMouseMoveFunction([this, &cubeRotation](float xpos, float ypos) {
			if (mouseRightHeld) {
				if (mouseLeftHeld) {
					if (firstMouse) {
						lastMouseX = xpos;
						lastMouseY = ypos;
						firstMouse = false;
					}

					float xoffset = xpos - lastMouseX;
					float yoffset = ypos - lastMouseY;
					lastMouseX = xpos;
					lastMouseY = ypos;
					float camSense = 0.05f;
					xoffset *= camSense;
					yoffset *= camSense;

					float newYaw = cam.getYaw() + xoffset;
					cam.setYaw(newYaw);
					float newPitch = cam.getPitch() + yoffset;

					// Constrain the pitch to avoid flipping
					if (cam.getPitch() > 89.0f)
						cam.setPitch(89.0f);
					if (cam.getPitch() < -89.0f)
						cam.setPitch(-89.0f);

					// Update camera direction
					cam.rotate(xoffset, -yoffset);
				}
				else {
					// Reset firstMouse when the mouse button is released
					firstMouse = true;
				}
			}

			else if (!mouseRightHeld) {
				if (mouseLeftHeld) {
					float xoffset = xpos - lastMouseX;
					float yoffset = ypos - lastMouseY;

					lastMouseX = xpos;
					lastMouseY = ypos;

					float cubeSense = 0.05f;
					xoffset *= cubeSense;
					yoffset *= cubeSense;

					cubeRotation.y += xoffset;
					cubeRotation.x += yoffset;

					mat4 cubeRotationMatrix = rotationaxis(vec3(1, 0, 0), cubeRotation.x) *
						rotationaxis(vec3(0, 1, 0), cubeRotation.y);
					cubeNode->setRotation(cubeRotationMatrix);
					
				}
			}
		});


		window->SetMousePressFunction([this](int button, int action, int mods) {
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
				mouseLeftHeld = true;
			}
			else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
				mouseLeftHeld = false;
			}
			if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
				mouseRightHeld = true;
			}
			else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
				mouseRightHeld = false;
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
	mat4 rotationMatrix;
	mat4 viewProjectionMatrix = cam.getProjectionMatrix() * cam.getViewMatrix();
	meshTestNode->setScale(vec3(0.25, 0.25, 0.25));
	std::cout << "Camera Position: " << cam.getPosition().x << cam.getPosition().y << cam.getPosition().z << std::endl;
	std::cout << "Camera Target: " << cam.getTarget().x << cam.getTarget().y << cam.getTarget().z << std::endl;
	std::cout << "Camera Up: " << cam.getUp().x << cam.getUp().y << cam.getUp().z << std::endl;
	while (this->window->IsOpen()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->window->Update();

		// do stuff
		viewProjectionMatrix = cam.getProjectionMatrix() * cam.getViewMatrix();
		cubeNode->draw(cam);
		meshTestNode->draw(cam);
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