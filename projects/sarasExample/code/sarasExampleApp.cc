#include "config.h"
#include "sarasExampleApp.h"
#include <fstream>
#include <sstream>
#include "math/mat4.h"
using namespace Display;
namespace Example
{
ExampleApp::ExampleApp() {}

ExampleApp::~ExampleApp() {}

bool ExampleApp::Open() {
	App::Open();
	this->window = new Display::Window;

	if (this->window->Open()) {
		std::shared_ptr<MeshResource> bunnyMesh = MeshResource::loadFromOBJ("assets/bunny.obj");
		if (!bunnyMesh) {
			std::cerr << "Failed to load bunny from OBJ" << std::endl;
			return false;
		}
		else {
			std::cout << "Bunny loaded from OBJ" << std::endl;
		}

		std::shared_ptr<MeshResource> cubeMesh = MeshResource::createCube(0.5f, 0.5f, 0.5f);
		if (!cubeMesh) {
			std::cerr << "Failed to create cube mesh" << std::endl;
			return false;
		}
		else {
			std::cout << "Cube created" << std::endl;
		}

		std::shared_ptr<MeshResource> horseMesh = MeshResource::loadFromOBJ("assets/horse_conv.obj");
		if (!horseMesh) { 
			std::cerr << "Failed to load horse from OBJ" << std::endl;
			return false;
		}
		else {
			std::cout << "Horse loaded from OBJ" << std::endl;
		}

		std::shared_ptr<MeshResource> lightMesh = MeshResource::loadFromOBJ("assets/light.obj");
		if (!lightMesh) {
			std::cerr << "Failed to load lightMesh from OBJ" << std::endl;
			return false;
		}
		else {
			std::cout << "Light Mesh loaded from OBJ" << std::endl;
		}

		std::shared_ptr<MeshResource> cubeGLTF = MeshResource::loadGLTF("assets/Cube/glTF/Cube.gltf");
		if (!cubeGLTF) {
			std::cerr << "Failed to load cude from GLTF" << std::endl;
			return false;
		}
		else {
			std::cout << "Cude loaded from GLTF" << std::endl;
		}

		std::shared_ptr<MeshResource> avocadoGLTF = MeshResource::loadGLTF("assets/Avocado/glTF/Avocado.gltf");
		if (!avocadoGLTF) {
			std::cerr << "Failed to load avocado from GLTF" << std::endl;
			return false;
		}
		else {
			std::cout << "Avocado loaded" << std::endl;
		}

		std::shared_ptr<MeshResource> damagedHelmet = MeshResource::loadGLTF("assets/DamagedHelmet/glTF-Embedded/DamagedHelmet.gltf");
		if (!damagedHelmet) {
			std::cerr << "Failed to load damagedHelmet" << std::endl;
			return false;
		}
		else {
			std::cout << "DamedHelmet loaded" << std::endl;
		}

		std::shared_ptr<MeshResource> fightHelmet = MeshResource::loadGLTF("assets/FlightHelmet/glTF/FlightHelmet.gltf");
		if (!fightHelmet) {
			std::cerr << "Failed to load fightHelmet" << std::endl;
			return false;
		}
		else {
			std::cout << "FightHelmet loaded" << std::endl;
		}
		
		// Loading shader
		std::shared_ptr<ShaderResource> shader = std::make_shared<ShaderResource>();
		//std::string shaderPath1 = "assets/shader.txt";
		std::string shaderPath2 = "assets/blinn_phong.shader";
		shader->load(shaderPath2);

		std::shared_ptr<BlinnPhongMaterial> woodMaterial = std::make_shared<BlinnPhongMaterial>(shader);
		std::shared_ptr<BlinnPhongMaterial> plasticMaterial = std::make_shared<BlinnPhongMaterial>(shader);
		std::shared_ptr<BlinnPhongMaterial> HorseMaterial = std::make_shared<BlinnPhongMaterial>(shader);
		std::shared_ptr<BlinnPhongMaterial> lampMaterial = std::make_shared<BlinnPhongMaterial>(shader);
		std::shared_ptr<BlinnPhongMaterial> GLTFCubeMaterial = std::make_shared<BlinnPhongMaterial>(shader);
		std::shared_ptr<BlinnPhongMaterial> AvocadoMaterial = std::make_shared<BlinnPhongMaterial>(shader);
		std::shared_ptr<BlinnPhongMaterial> dHelmetMaterial = std::make_shared<BlinnPhongMaterial>(shader);
		std::shared_ptr<BlinnPhongMaterial> fHelmetMaterial = std::make_shared<BlinnPhongMaterial>(shader);

		// Loading texture
		std::shared_ptr<TextureResource> woodTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> woodSpecTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> rubikTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> rubikSpecTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> horseTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> horseSpecTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> discoTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> discoSpecTex = std::make_shared<TextureResource>();

		std::shared_ptr<TextureResource> GLTFCubeTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> cubeMetallicTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> avocadoTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> avocadoMetallicTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> dHelmetTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> dHelmetSpecTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> dHelmetMetallicTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> dHelmetEmissiveTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> fHelmetTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> fHelmetMetallicTex = std::make_shared<TextureResource>();

		woodTex->load("assets/wood.jpg");
		woodSpecTex->load("assets/wood_spec.jpg");
		rubikTex->load("assets/Rubik2.png");
		rubikSpecTex->load("assets/Rubik2_spec.png");
		horseTex->load("assets/horse.jpg", 1);
		horseSpecTex->load("assets/horse_spec2.jpg", 1);
		discoTex->load("assets/disco2.jpg");
		discoSpecTex->load("assets/disco2_spec.jpg");

		GLTFCubeTex->load("assets/Cube/glTF/Cube_BaseColor.png");
		cubeMetallicTex->load("assets/Cube/glTF/Cube_MetallicRoughness.png");
		avocadoTex->load("assets/Avocado/glTF/Avocado_baseColor.png");
		avocadoMetallicTex->load("assets/Avocado/glTF/Avocado_roughnessMetallic.png");
		dHelmetTex->load("assets/DamagedHelmet/glTF/Default_albedo.jpg", 1);
		dHelmetSpecTex->load("assets/DamagedHelmet/glTF/Default_AO.jpg", 1);
		dHelmetMetallicTex->load("assets/DamagedHelmet/glTF/Default_metalRoughness.jpg", 1);
		dHelmetEmissiveTex->load("assets/DamagedHelmet/glTF/Default_emissive.jpg", 1);
		fHelmetTex->load("assets/FlightHelmet/glTF/FlightHelmet_Materials_MetalPartsMat_BaseColor.png", 1);
		fHelmetMetallicTex->load("assets/FlightHelmet/glTF/FlightHelmet_Materials_MetalPartsMat_OcclusionRoughMetal.png", 1);

		// Material properties
		woodMaterial->setDiffuseTexture(woodTex);
		woodMaterial->setSpecularTexture(woodSpecTex);
		woodMaterial->setShininess(16.0f);

		plasticMaterial->setDiffuseTexture(rubikTex);
		plasticMaterial->setSpecularTexture(rubikSpecTex);
		plasticMaterial->setShininess(32.0f);

		HorseMaterial->setDiffuseTexture(horseTex);
		HorseMaterial->setSpecularTexture(horseSpecTex);
		HorseMaterial->setShininess(1.0f);

		lampMaterial->setDiffuseTexture(discoTex);
		lampMaterial->setSpecularTexture(discoSpecTex);
		lampMaterial->setShininess(64.0f);

		GLTFCubeMaterial->setDiffuseTexture(GLTFCubeTex);
		GLTFCubeMaterial->setSpecularTexture(cubeMetallicTex);
		GLTFCubeMaterial->setShininess(32.0f);

		AvocadoMaterial->setDiffuseTexture(avocadoTex);
		AvocadoMaterial->setMetallicTexture(avocadoMetallicTex);
		AvocadoMaterial->setShininess(32.0f);

		dHelmetMaterial->setDiffuseTexture(dHelmetTex);
		dHelmetMaterial->setSpecularTexture(dHelmetSpecTex);
		dHelmetMaterial->setSpecularTexture(dHelmetMetallicTex);
		dHelmetMaterial->setEmissiveTexture(dHelmetEmissiveTex);
		dHelmetMaterial->setShininess(16.0f);

		fHelmetMaterial->setDiffuseTexture(fHelmetTex);
		fHelmetMaterial->setMetallicTexture(fHelmetMetallicTex);
		fHelmetMaterial->setShininess(32.0f);

		// Creating a GraphicsNodes to manage the meshes
		bunnyNode = std::make_shared<GraphicsNode>();
		cubeNode = std::make_shared<GraphicsNode>();
		horseNode = std::make_shared<GraphicsNode>();
		lightNode = std::make_shared<GraphicsNode>();
		GLTFCubeNode = std::make_shared<GraphicsNode>();
		avocadoNode = std::make_shared<GraphicsNode>();
		damagedHelmetNode = std::make_shared<GraphicsNode>();
		fightHelmetNode = std::make_shared<GraphicsNode>();

		bunnyNode->setMesh(bunnyMesh);
		bunnyNode->setShader(shader);
		bunnyNode->setMaterial(woodMaterial);

		cubeNode->setMesh(cubeMesh);
		cubeNode->setShader(shader);
		cubeNode->setMaterial(plasticMaterial);

		horseNode->setMesh(horseMesh);
		horseNode->setShader(shader);
		horseNode->setMaterial(HorseMaterial);

		lightNode->setMesh(lightMesh);
		lightNode->setShader(shader);
		lightNode->setMaterial(lampMaterial);

		GLTFCubeNode->setMesh(cubeGLTF);
		GLTFCubeNode->setShader(shader);
		GLTFCubeNode->setMaterial(GLTFCubeMaterial);

		avocadoNode->setMesh(avocadoGLTF);
		avocadoNode->setShader(shader);
		avocadoNode->setMaterial(AvocadoMaterial);

		damagedHelmetNode->setMesh(damagedHelmet);
		damagedHelmetNode->setShader(shader);
		damagedHelmetNode->setMaterial(dHelmetMaterial);

		fightHelmetNode->setMesh(fightHelmet);
		fightHelmetNode->setShader(shader);
		fightHelmetNode->setMaterial(fHelmetMaterial);

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
			vec4 bunnyMovement(0, 0, 0, 0);
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
					case GLFW_KEY_UP: bunnyMovement.z -= moveSpeed; break;
					case GLFW_KEY_DOWN: bunnyMovement.z += moveSpeed; break;
					case GLFW_KEY_LEFT: bunnyMovement.x -= moveSpeed; break;
					case GLFW_KEY_RIGHT: bunnyMovement.x += moveSpeed; break;
					}
				}
				if (action == GLFW_PRESS || action == GLFW_REPEAT) {
					switch (key) {
					case GLFW_KEY_W: cubeMovement.z -= moveSpeed; break;
					case GLFW_KEY_S: cubeMovement.z += moveSpeed; break;
					case GLFW_KEY_A: cubeMovement.x -= moveSpeed; break;
					case GLFW_KEY_D: cubeMovement.x += moveSpeed; break;
					}
				}
			}
			cam.setPosition(cam.getPosition() + camMovement);
			cam.updateCameraVectors();
			bunnyNode->setPosition(bunnyNode->getPosition() + bunnyMovement);
			cubeNode->setPosition(cubeNode->getPosition() + cubeMovement);
		});

		vec3 cubeRotation(0.0f, 0.0f, 0.0f);
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

					mat4 cubeRotationMatrix = rotationaxis(vec3(1.0f, 0.0f, 0.0f), cubeRotation.x) *
						rotationaxis(vec3(0.0f, 1.0f, 0.0f), cubeRotation.y);
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
 
void ExampleApp::Close() {
	if (this->window->IsOpen())
		this->window->Close();
	delete grid;

	Core::App::Close();
}

void ExampleApp::Run() {
	glEnable(GL_DEPTH_TEST);
	float speed = 0.0f;
	float radius = 2.0f;
	float elapsedTime = 0.0f;
	float pauseTime = 0.0f;
	float lastPauseStart = 0.0f;
	bool isPaused = false;
	float degrees360 = 3.14f * 2.0f;

	mat4 bunnyRotationMatrix;
	mat4 cubeRotationMatrix;
	mat4 horseRotationMatrix = rotationaxis(vec3(1.0f, 0.0f, 0.0f), -degrees360 * 0.25f);
	mat4 GLTFCubeRotationMatrix;
	mat4 avocadoRotationMatrix = 
		rotationaxis(vec3(0.0f, 1.0f, 0.0f), degrees360 * 0.4f) * 
		rotationaxis(vec3(1.0f, 0.0f, 0.0f), degrees360 * 0.25f);
	mat4 dhemletRotationMatrix =
		rotationaxis(vec3(1.0f, 0.0f, 0.0f), degrees360 * 0.245f);
	mat4 fhemletRotationMatrix;
	mat4 viewProjectionMatrix = cam.getProjectionMatrix() * cam.getViewMatrix();

	bunnyNode->setScale(vec3(0.25f, 0.25f, 0.25f));
	bunnyNode->setRotation(bunnyRotationMatrix);
	bunnyNode->setPosition(vec4(0.5f, -0.006f, -0.5f, 1.0f));

	cubeNode->setScale(vec3(0.5f, 0.5f, 0.5f));
	cubeNode->setRotation(cubeRotationMatrix);
	cubeNode->setPosition(vec4(0.0f, 0.125f, 0.0f, 1.0f));

	horseNode->setScale(vec3(0.05f, 0.05f, 0.05f));
	horseNode->setRotation(horseRotationMatrix);
	horseNode->setPosition(vec4(-0.5f, 0.0f, -0.5f, 1.0f));
	
	lightNode->setScale(vec3(0.05f, 0.05f, 0.05f));

	GLTFCubeNode->setScale(vec3(0.125f, 0.125f, 0.125f));
	GLTFCubeNode->setRotation(GLTFCubeRotationMatrix);
	GLTFCubeNode->setPosition(vec4(-0.5f, 0.1245f, 0.5f, 1.0f));

	avocadoNode->setScale(vec3(5.0f, 5.0f, 5.0f));
	avocadoNode->setRotation(avocadoRotationMatrix);
	avocadoNode->setPosition(vec4(0.0f, 0.07f, 0.5f, 1.0f));

	damagedHelmetNode->setScale(vec3(0.2f, 0.2f, 0.2f));
	damagedHelmetNode->setRotation(dhemletRotationMatrix);
	damagedHelmetNode->setPosition(vec4(0.5f, 0.17f, 0.5f, 1.0f));

	fightHelmetNode->setScale(vec3(1.0f, 1.0f, 1.0f));
	fightHelmetNode->setRotation(fhemletRotationMatrix);
	fightHelmetNode->setPosition(vec4(1.0f, 0.0f, 0.5f, 1.0f));

	float initialTime = float(glfwGetTime());
	while (this->window->IsOpen()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->window->Update();
		float currentTime = float(glfwGetTime());
		
		// Light-pause mekanism
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
			elapsedTime = float(glfwGetTime()) - initialTime - pauseTime;
			speed = elapsedTime;
			vec4 newLightPos(radius * cosf(speed), light.getPointLightPos().y, radius * sinf(speed), 1.0f);
			light.setPosition(vec3(newLightPos.x, newLightPos.y, newLightPos.z));
			lightNode->setPosition(newLightPos);
		}

		viewProjectionMatrix = cam.getProjectionMatrix() * cam.getViewMatrix();

		bunnyNode->draw(cam, light);
		cubeNode->draw(cam, light);
		horseNode->draw(cam, light);
		lightNode->draw(cam, light);
		GLTFCubeNode->draw(cam, light);
		avocadoNode->draw(cam, light);
		damagedHelmetNode->draw(cam, light);
		fightHelmetNode->draw(cam, light);
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