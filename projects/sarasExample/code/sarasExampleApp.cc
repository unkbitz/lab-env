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
		std::shared_ptr<MeshResource> bunnyMesh = MeshResource::loadFromOBJ("assets/bunny.obj");
		if (!bunnyMesh) {
			std::cerr << "Failed to load OBJ mesh" << std::endl;
			return false;
		}

		std::shared_ptr<MeshResource> cubeMesh = MeshResource::createCube(0.5f, 0.5f, 0.5f);
		if (!cubeMesh) {
			std::cerr << "Failed to create cube mesh" << std::endl;
			return false;
		}

		std::shared_ptr<MeshResource> deerMesh = MeshResource::loadFromOBJ("assets/lowpolydeer/deer.obj");
		if (!deerMesh) { 
			std::cerr << "Failed to load OBJ mesh" << std::endl;
			return false;
		}

		std::shared_ptr<MeshResource> lightMesh = MeshResource::loadFromOBJ("assets/light.obj");
		if (!lightMesh) {
			std::cerr << "Failed to load OBJ mesh" << std::endl;
			return false;
		}
		
		//Loading shader
		std::shared_ptr<ShaderResource> shader = std::make_shared<ShaderResource>();
		std::string shaderPath1 = "assets/shader.txt";
		std::string shaderPath2 = "assets/blinn_phong.txt";
		shader->load(shaderPath2);

		//Loading texture
		std::shared_ptr<TextureResource> woodTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> rubikTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> brownTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> discoTex = std::make_shared<TextureResource>();

		woodTex->load("assets/wood.jpg");
		rubikTex->load("assets/Rubik2.png");
		brownTex->load("assets/brown.png");
		discoTex->load("assets/disco2.jpg");

		//Creating a GraphicsNodes to manage the meshes
		bunnyNode = std::make_shared<GraphicsNode>();
		cubeNode = std::make_shared<GraphicsNode>();
		deerNode = std::make_shared<GraphicsNode>();
		lightNode = std::make_shared<GraphicsNode>();

		bunnyNode->setMesh(bunnyMesh);
		bunnyNode->setShader(shader);
		bunnyNode->setTexture(woodTex);

		cubeNode->setMesh(cubeMesh);
		cubeNode->setShader(shader);
		cubeNode->setTexture(rubikTex);

		deerNode->setMesh(deerMesh);
		deerNode->setShader(shader);
		deerNode->setTexture(brownTex);

		lightNode->setMesh(lightMesh);
		lightNode->setShader(shader);
		lightNode->setTexture(discoTex);

		grid = new Render::Grid();
		  
		// set clear color to gray
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		window->SetKeyPressFunction([this](int key, int scancode, int action, int mods) {
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
				this->window->Close();
			}
			if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
				if (!lightPause) {
					lightPause = true;
					std :: cout << "Light movement paused" << std::endl;
				}
				else if (lightPause) {
					lightPause = false;
					std::cout << "Light movement restarted" << std::endl;
				}
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
			bunnyNode->setPosition(bunnyNode->getPosition() + meshMovement);
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

	float speed = 0.0f;
	float radius = 1.0f;
	float elapsedTime = 0.0f;
	float pauseTime = 0.0f;
	float lastPauseStart = 0.0f;
	bool isPaused = false;

	mat4 bunnyRotationMatrix;
	mat4 cubeRotationMatrix;
	mat4 deerRotationMatrix;
	
	mat4 viewProjectionMatrix = cam.getProjectionMatrix() * cam.getViewMatrix();

	bunnyNode->setScale(vec3(0.25, 0.25, 0.25));
	bunnyNode->setRotation(bunnyRotationMatrix);
	bunnyNode->setPosition(vec4(0.5, -0.006, -0.5, 1.0));

	cubeNode->setScale(vec3(0.5, 0.5, 0.5));
	cubeNode->setRotation(cubeRotationMatrix);
	cubeNode->setPosition(vec4(0.0, 0.125, 0.0, 1.0));

	deerNode->setScale(vec3(0.001, 0.001, 0.001));
	deerNode->setRotation(deerRotationMatrix);
	deerNode->setPosition(vec4(-0.5, 0.0, -0.5, 1.0));
	
	lightNode->setScale(vec3(0.05, 0.05, 0.05));

	float initialTime = glfwGetTime();
	while (this->window->IsOpen()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->window->Update();
		
		float currentTime = glfwGetTime();
		
		if (lightPause && !isPaused) {
			// If paused, track when the pause started
			lastPauseStart = currentTime;
			isPaused = true;
		}
		if (!lightPause && isPaused) {
			// Caulculate time spent paused
			pauseTime += currentTime - lastPauseStart;
			isPaused = false;
		}
		if (!lightPause) {
			elapsedTime = glfwGetTime() - initialTime - pauseTime;
			speed = elapsedTime;
			vec4 newLightPos(radius * cosf(speed), light.getPointLightPos().y, radius * sinf(speed), 1.0f);
			light.setPosition(vec3(newLightPos.x, newLightPos.y, newLightPos.z));
			lightNode->setPosition(newLightPos);
		}

		viewProjectionMatrix = cam.getProjectionMatrix() * cam.getViewMatrix();

		bunnyNode->draw(cam, light);
		cubeNode->draw(cam, light);
		deerNode->draw(cam, light);
		lightNode->draw(cam, light);

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