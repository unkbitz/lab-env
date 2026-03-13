#include "config.h"
#include "sarasExampleApp.h"
#include <fstream>
#include <sstream>
#include "math/mat4.h"
#include <random>

using namespace Display;
namespace Example
{
ExampleApp::ExampleApp() {}

ExampleApp::~ExampleApp() {}

bool ExampleApp::Open() {
	App::Open();
	this->window = new Display::Window;

	if (this->window->Open()) {

		int width;
		int height;
		this->window->GetSize(width, height);

		this->gBuffer = std::make_unique<GBuffer>();

		if (!this->gBuffer->initialize(width, height))
		{
			std::cerr << "Failed to initialize GBuffer" << std::endl;
			return false;
		}

		float quadVertices[] =
		{
			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 1.0f, 1.0f,

			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 1.0f
		};

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);

		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		std::shared_ptr<MeshResource> bunnyMesh = MeshResource::loadFromOBJ("assets/bunny.obj");
		if (!bunnyMesh) {
			std::cerr << "Failed to load bunny from OBJ" << std::endl;
			return false;
		}
		else {
			std::cout << "Bunny loaded from OBJ" << std::endl;
		}

		std::shared_ptr<MeshResource> ambulanceMesh = MeshResource::loadFromOBJ("assets/kenney_car-kit/Models/OBJ format/ambulance.obj");
		if (!ambulanceMesh) {
			std::cerr << "Failed to load ambulanceMesh from OBJ" << std::endl;
			return false;
		}
		else {
			std::cout << "ambulanceMesh loaded from OBJ" << std::endl;
		}

		std::shared_ptr<MeshResource> deliveryMesh = MeshResource::loadFromOBJ("assets/kenney_car-kit/Models/OBJ format/delivery.obj");
		if (!deliveryMesh) {
			std::cerr << "Failed to load deliveryMesh from OBJ" << std::endl;
			return false;
		}
		else {
			std::cout << "ambulanceMesh loaded from OBJ" << std::endl;
		}

		std::shared_ptr<MeshResource> fireTruckMesh = MeshResource::loadFromOBJ("assets/kenney_car-kit/Models/OBJ format/firetruck.obj");
		if (!fireTruckMesh) {
			std::cerr << "Failed to load fireTruckMesh from OBJ" << std::endl;
			return false;
		}
		else {
			std::cout << "ambulanceMesh loaded from OBJ" << std::endl;
		}

		std::shared_ptr<MeshResource> raceMesh = MeshResource::loadFromOBJ("assets/kenney_car-kit/Models/OBJ format/race.obj");
		if (!raceMesh) {
			std::cerr << "Failed to load race from OBJ" << std::endl;
			return false;
		}
		else {
			std::cout << "ambulanceMesh loaded from OBJ" << std::endl;
		}

		std::shared_ptr<MeshResource> taxiMesh = MeshResource::loadFromOBJ("assets/kenney_car-kit/Models/OBJ format/taxi.obj");
		if (!taxiMesh) {
			std::cerr << "Failed to load taxiMesh from OBJ" << std::endl;
			return false;
		}
		else {
			std::cout << "ambulanceMesh loaded from OBJ" << std::endl;
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

		// Loading shader
		std::shared_ptr<ShaderResource> shader = std::make_shared<ShaderResource>();
		//std::string shaderPath1 = "assets/shader.txt";
		std::string shaderPath2 = "assets/blinn_phong.shader";
		shader->load(shaderPath2);


		geometryShader = std::make_shared<ShaderResource>();
		std::string shaderPathGeometry = "assets/deferred_geometry.shader";
		geometryShader->load(shaderPathGeometry);

		debugShader = std::make_shared<ShaderResource>();
		debugShader->load("assets/debug_texture.shader");

		lightDebugShader = std::make_shared<ShaderResource>();
		lightDebugShader->load("assets/light_debug.shader");

		lightingShader = std::make_shared<ShaderResource>();
		lightingShader->load("assets/deferred_lighting.shader");


		std::shared_ptr<BlinnPhongMaterial> woodMaterial = std::make_shared<BlinnPhongMaterial>(geometryShader, 16.0f);
		std::shared_ptr<BlinnPhongMaterial> plasticMaterial = std::make_shared<BlinnPhongMaterial>(geometryShader, 32.0f);
		std::shared_ptr<BlinnPhongMaterial> HorseMaterial = std::make_shared<BlinnPhongMaterial>(geometryShader, 1.0f);
		std::shared_ptr<BlinnPhongMaterial> lampMaterial = std::make_shared<BlinnPhongMaterial>(lightDebugShader, 64.0f);
		std::shared_ptr<BlinnPhongMaterial> carMaterial = std::make_shared<BlinnPhongMaterial>(geometryShader, 32.0f);

		// Loading texture
		std::shared_ptr<TextureResource> woodTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> woodSpecTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> rubikTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> rubikSpecTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> horseTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> horseSpecTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> discoTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> discoSpecTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> ambulanceTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> raceTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> taxiTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> deliveryTex = std::make_shared<TextureResource>();
		std::shared_ptr<TextureResource> fireTruckTex = std::make_shared<TextureResource>();

		woodTex->loadTextureURI("assets/wood.jpg", 0);
		woodSpecTex->loadTextureURI("assets/wood_spec.jpg", 0);
		rubikTex->loadTextureURI("assets/Rubik2.png", 0);
		rubikSpecTex->loadTextureURI("assets/Rubik2_spec.png", 0);
		horseTex->loadTextureURI("assets/horse.jpg", 1);
		horseSpecTex->loadTextureURI("assets/horse_spec2.jpg", 1);
		discoTex->loadTextureURI("assets/disco2.jpg", 0);
		discoSpecTex->loadTextureURI("assets/disco2_spec.jpg", 0);
		//ambulanceTex->loadTextureURI("assets/disco2_spec.jpg", 0);
		//raceTex->loadTextureURI("assets/disco2_spec.jpg", 0);
		//deliveryTex->loadTextureURI("assets/disco2_spec.jpg", 0);
		//taxiTex->loadTextureURI("assets/disco2_spec.jpg", 0);
		//fireTruckTex->loadTextureURI("assets/disco2_spec.jpg", 0);

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

		lampMaterial->setDiffuseTexture(nullptr);
		lampMaterial->setSpecularTexture(nullptr);
		lampMaterial->setShininess(64.0f);

		carMaterial->setDiffuseTexture(nullptr);
		carMaterial->setSpecularTexture(nullptr);
		carMaterial->setShininess(32.0f);

		// Creating a GraphicsNodes to manage the meshes
		bunnyNode = std::make_shared<GraphicsNode>();
		cubeNode = std::make_shared<GraphicsNode>();
		horseNode = std::make_shared<GraphicsNode>();
		ambulanceNode = std::make_shared<GraphicsNode>();
		raceNode = std::make_shared<GraphicsNode>();
		taxiNode = std::make_shared<GraphicsNode>();
		deliveryNode = std::make_shared<GraphicsNode>();
		fireTruckNode = std::make_shared<GraphicsNode>();

		GLTFCubeNode = GLTFLoader::loadGLTFRootNode(
			"assets/Cube/glTF/Cube.gltf", 
			"assets/Cube/glTF/",
			shaderPathGeometry,
			8.0f, 1);
		avocadoNode = GLTFLoader::loadGLTFRootNode(
			"assets/Avocado/glTF/Avocado.gltf", 
			"assets/Avocado/glTF/",
			shaderPathGeometry,
			16.0f,
			0);
		damagedHelmetNode = GLTFLoader::loadGLTFRootNode(
			"assets/DamagedHelmet/glTF/DamagedHelmet.gltf", 
			"assets/DamagedHelmet/glTF/",
			shaderPathGeometry,
			32.0f,
			1);
		flightHelmetNode = GLTFLoader::loadGLTFRootNode(
			"assets/FlightHelmet/glTF/FlightHelmet.gltf", 
			"assets/FlightHelmet/glTF/",
			shaderPathGeometry,
			32.0f,
			0);
		normalTangentMirrorNode = GLTFLoader::loadGLTFRootNode(
			"assets/NormalTangentMirrorTest/glTF/NormalTangentMirrorTest.gltf", 
			"assets/NormalTangentMirrorTest/glTF/",
			shaderPathGeometry,
			64.0f,
			0);
		rubberDuckNode = GLTFLoader::loadGLTFRootNode(
			"assets/rubber_duck.gltf/rubber_duck_toy_4k.gltf",
			"assets/rubber_duck.gltf/",
			shaderPathGeometry,
			64.0f,
			0);
		horseHeadNode = GLTFLoader::loadGLTFRootNode(
			"assets/horse_head_4k.gltf/horse_head_4k.gltf",
			"assets/horse_head_4k.gltf/",
			shaderPathGeometry,
			64.0f,
			0);
		lionHeadNode = GLTFLoader::loadGLTFRootNode(
			"assets/lion_head_4k.gltf/lion_head_4k.gltf",
			"assets/lion_head_4k.gltf/",
			shaderPathGeometry,
			64.0f,
			0);
		elephantNode = GLTFLoader::loadGLTFRootNode(
			"assets/carved_wooden_elephant_4k.gltf/carved_wooden_elephant_4k.gltf",
			"assets/carved_wooden_elephant_4k.gltf/",
			shaderPathGeometry,
			32.0f,
			0);
		kiwiNode = GLTFLoader::loadGLTFRootNode(
			"assets/food_kiwi_01_4k.gltf/food_kiwi_01_4k.gltf",
			"assets/food_kiwi_01_4k.gltf/",
			shaderPathGeometry,
			8.0f,
			0);
		pomegranateNode = GLTFLoader::loadGLTFRootNode(
			"assets/food_pomegranate_01_4k.gltf/food_pomegranate_01_4k.gltf",
			"assets/food_pomegranate_01_4k.gltf/",
			shaderPathGeometry,
			16.0f,
			0);
		appleNode = GLTFLoader::loadGLTFRootNode(
			"assets/food_apple_01_4k.gltf/food_apple_01_4k.gltf",
			"assets/food_apple_01_4k.gltf/",
			shaderPathGeometry,
			32.0f,
			0);
		//sponzaNode = GLTFLoader::loadGLTFRootNode("assets/Sponza/glTF/Sponza.gltf", "assets/Sponza/glTF/", 32.0f, 0);

		bunnyNode->setMesh(bunnyMesh);
		bunnyNode->setShader(geometryShader);
		bunnyNode->setMaterial(woodMaterial);

		cubeNode->setMesh(cubeMesh);
		cubeNode->setShader(geometryShader);
		cubeNode->setMaterial(plasticMaterial);

		horseNode->setMesh(horseMesh);
		horseNode->setShader(geometryShader);
		horseNode->setMaterial(HorseMaterial);

		ambulanceNode->setMesh(ambulanceMesh);
		ambulanceNode->setShader(geometryShader);
		ambulanceNode->setMaterial(carMaterial);

		taxiNode->setMesh(taxiMesh);
		taxiNode->setShader(geometryShader);
		taxiNode->setMaterial(carMaterial);

		raceNode->setMesh(raceMesh);
		raceNode->setShader(geometryShader);
		raceNode->setMaterial(carMaterial);

		deliveryNode->setMesh(deliveryMesh);
		deliveryNode->setShader(geometryShader);
		deliveryNode->setMaterial(carMaterial);

		fireTruckNode->setMesh(fireTruckMesh);
		fireTruckNode->setShader(geometryShader);
		fireTruckNode->setMaterial(carMaterial);

		for (int i = 0; i < 20; i++)
		{
			Lighting light;

			float r = (rand() % 100) / 100.0f;
			float g = (rand() % 100) / 100.0f;
			float b = (rand() % 100) / 100.0f;

			float x = ((rand() % 200) / 10.0f) - 10.0f;
			float y = ((rand() % 30) / 10.0f) + 0.5f;
			float z = ((rand() % 200) / 10.0f) - 10.0f;

			std::shared_ptr<GraphicsNode> lightNode = std::make_shared<GraphicsNode>();

			lightNode->setMesh(lightMesh);
			lightNode->setShader(lightDebugShader);
			lightNode->setMaterial(lampMaterial);

			lightNode->setScale(vec3(0.05f, 0.05f, 0.05f));
			lightNode->setBaseColor(vec4(r, g, b, 1.0f));

			light.setColor(vec3(r, g, b));
			light.setPosition(vec3(x, y, z));

			lightNode->setPosition({ x,y,z,0 });

			lights.push_back(light);
			lightNodes.push_back(lightNode);
		}

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
	mat4 rubberDuckMatrix;
	mat4 horseHeadMatrix;
	mat4 lionHeadMatrix;
	mat4 elephantMatrix;
	mat4 kiwiMatrix;
	mat4 pomegranateMatrix;
	mat4 appleMatrix;
	mat4 carMatrix = rotationaxis(vec3(0.0f, 1.0f, 0.0f), -degrees360 * 0.125f);
	mat4 sponzaRotationMatrix;
	mat4 viewProjectionMatrix = cam.getProjectionMatrix() * cam.getViewMatrix();

	// setPosition(x = right/left, y = up/down, z = in/out, 1.0f)
	bunnyNode->setScale(vec3(0.25f, 0.25f, 0.25f));
	bunnyNode->setRotation(bunnyRotationMatrix);
	bunnyNode->setPosition(vec4(0.5f, -0.006f, -0.5f, 1.0f));

	cubeNode->setScale(vec3(0.5f, 0.5f, 0.5f));
	cubeNode->setRotation(cubeRotationMatrix);
	cubeNode->setPosition(vec4(0.0f, 0.125f, 0.0f, 1.0f));

	horseNode->setScale(vec3(0.05f, 0.05f, 0.05f));
	horseNode->setRotation(horseRotationMatrix);
	horseNode->setPosition(vec4(-0.5f, 0.0f, -0.5f, 1.0f));
	
	ambulanceNode->setScale(vec3(0.5f, 0.5f, 0.5f));
	ambulanceNode->setRotation(carMatrix);
	ambulanceNode->setPosition(vec4(-3.0f, 0.0f, 2.0f, 1.0f));
	ambulanceNode->setUseDiffuseTexture(false);
	ambulanceNode->setBaseColor(vec4(0.5f, 0.0f, 7.0f, 1.0f));

	raceNode->setScale(vec3(0.5f, 0.5f, 0.5f));
	raceNode->setRotation(carMatrix);
	raceNode->setPosition(vec4(-1.5f, 0.0f, 2.0f, 1.0f));
	raceNode->setUseDiffuseTexture(false);
	raceNode->setBaseColor(vec4(0.5f, 0.7f, 0.0f, 1.0f));

	taxiNode->setScale(vec3(0.5f, 0.5f, 0.5f));
	taxiNode->setRotation(carMatrix);
	taxiNode->setPosition(vec4(0.0f, 0.0f, 2.0f, 1.0f));
	taxiNode->setUseDiffuseTexture(false);
	taxiNode->setBaseColor(vec4(1.0f, 1.0f, 0.0f, 1.0f));

	fireTruckNode->setScale(vec3(0.5f, 0.5f, 0.5f));
	fireTruckNode->setRotation(carMatrix);
	fireTruckNode->setPosition(vec4(1.5f, 0.0f,2.0f, 1.0f));
	fireTruckNode->setUseDiffuseTexture(false);
	fireTruckNode->setBaseColor(vec4(0.8f, 0.0f, 0.0f, 1.0f));

	deliveryNode->setScale(vec3(0.5f, 0.5f, 0.5f));
	deliveryNode->setRotation(carMatrix);
	deliveryNode->setPosition(vec4(3.0f, 0.0f, 2.0f, 1.0f));
	deliveryNode->setUseDiffuseTexture(false);
	deliveryNode->setBaseColor(vec4(0.0f, 0.7f, 0.5f, 1.0f));

	GLTFCubeNode->setScale(vec3(0.125f, 0.125f, 0.125f));
	GLTFCubeNode->setRotation(GLTFCubeRotationMatrix);
	GLTFCubeNode->setPosition(vec4(-0.5f, 0.1245f, 0.5f, 1.0f));

	avocadoNode->setScale(vec3(5.0f, 5.0f, 5.0f));
	avocadoNode->setRotation(avocadoRotationMatrix);
	avocadoNode->setPosition(vec4(0.0f, 0.07f, 0.5f, 1.0f));

	damagedHelmetNode->setScale(vec3(0.2f, 0.2f, 0.2f));
	damagedHelmetNode->setRotation(dhemletRotationMatrix);
	damagedHelmetNode->setPosition(vec4(0.5f, 0.17f, 0.5f, 1.0f));

	rubberDuckNode->setScale(vec3(20.0f, 20.0f, 20.0f));
	rubberDuckNode->setRotation(rubberDuckMatrix);
	rubberDuckNode->setPosition(vec4(-5.0f, 0.0f, 0.0f, 1.0f));

	horseHeadNode->setScale(vec3(20.0f, 20.0f, 20.0f));
	horseHeadNode->setRotation(horseHeadMatrix);
	horseHeadNode->setPosition(vec4(5.0f, 0.0f, -5.0f, 1.0f));

	lionHeadNode->setScale(vec3(20.0f, 20.0f, 20.0f));
	lionHeadNode->setRotation(lionHeadMatrix);
	lionHeadNode->setPosition(vec4(0.0f, 0.0f, -5.0f, 1.0f));

	elephantNode->setScale(vec3(5.0f, 5.0f, 5.0f));
	elephantNode->setRotation(elephantMatrix);
	elephantNode->setPosition(vec4(-2.0f, 0.0f, -1.0f, 1.0f));

	flightHelmetNode->setScale(vec3(1.0f, 1.0f, 1.0f));
	flightHelmetNode->setRotation(fhemletRotationMatrix);
	flightHelmetNode->setPosition(vec4(1.0f, 0.0f, 0.5f, 1.0f));
	normalTangentMirrorNode->setPosition(vec4(0.0f, 1.0f, -1.0f, 1.0f));

	kiwiNode->setScale(vec3(1.0f, 1.0f, 1.0f));
	kiwiNode->setRotation(kiwiMatrix);
	kiwiNode->setPosition(vec4(2.0f, 0.0f, 0.5f, 1.0f));

	pomegranateNode->setScale(vec3(1.0f, 1.0f, 1.0f));
	pomegranateNode->setRotation(pomegranateMatrix);
	pomegranateNode->setPosition(vec4(2.0f, 0.0f, 0.0f, 1.0f));

	appleNode->setScale(vec3(1.0f, 1.0f, 1.0f));
	appleNode->setRotation(appleMatrix);
	appleNode->setPosition(vec4(2.0f, 0.0f, -0.5f, 1.0f));

	float initialTime = float(glfwGetTime());
	while (this->window->IsOpen())
	{
		this->window->Update();

		gBuffer->bindForWriting();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float currentTime = float(glfwGetTime());

		for (int i = 0; i < lights.size(); i++)
		{
			// Light-pause mekanism
			if (lightPause && !isPaused)
			{
				// If paused, track when the pause started
				lastPauseStart = currentTime;
				isPaused = true;
			}
			if (!lightPause && isPaused)
			{
				// Caulculate time spent paused
				pauseTime += currentTime - lastPauseStart;
				isPaused = false;
			}
			if (!lightPause)
			{
				elapsedTime = float(glfwGetTime()) - initialTime - pauseTime;
				//speed = elapsedTime * 0.005;
				//vec4 newLightPos(radius * cosf(speed), lights[i].getPointLightPos().y, radius * sinf(speed), 1.0f);
				float angle = 0.05 * elapsedTime * (0.3f + 0.5f * i) + i * 0.7f;
				float orbitRadius = 2.0f + 0.2f * i;
				float x = cosf(angle) * orbitRadius;
				float z = sinf(angle) * orbitRadius;
				float y = 1.0f + 0.3f * sinf(angle * 1.7f);

				vec4 newLightPos(x, y, z, 1.0f);

				lights[i].setPosition(vec3(newLightPos.x, newLightPos.y, newLightPos.z));
				lightNodes[i]->setPosition(newLightPos);

			}

		}

		viewProjectionMatrix = cam.getProjectionMatrix() * cam.getViewMatrix();

		bunnyNode->drawGeometry(cam);
		cubeNode->drawGeometry(cam);
		horseNode->drawGeometry(cam);
		ambulanceNode->drawGeometry(cam);
		raceNode->drawGeometry(cam);
		taxiNode->drawGeometry(cam);
		deliveryNode->drawGeometry(cam);
		fireTruckNode->drawGeometry(cam);
		GLTFCubeNode->drawGeometry(cam);
		avocadoNode->drawGeometry(cam);
		damagedHelmetNode->drawGeometry(cam);
		rubberDuckNode->drawGeometry(cam);
		horseHeadNode->drawGeometry(cam);
		lionHeadNode->drawGeometry(cam);
		elephantNode->drawGeometry(cam);
		flightHelmetNode->drawGeometry(cam);
		kiwiNode->drawGeometry(cam);
		pomegranateNode->drawGeometry(cam);
		appleNode->drawGeometry(cam);
		//sponzaNode->draw(cam, light);
		//normalTangentMirrorNode->draw(cam, light);
		//grid->Draw((GLfloat*)&viewProjectionMatrix[0][0]);

		int debugMode = 4;
		gBuffer->unbind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (debugMode >= 1 && debugMode <= 3)
		{
			glDisable(GL_DEPTH_TEST);
			debugShader->bind();
			glActiveTexture(GL_TEXTURE0);

			switch (debugMode)
			{
			case 1:
				glBindTexture(GL_TEXTURE_2D, gBuffer->getAlbedoTexture());
				break;
			case 2:
				glBindTexture(GL_TEXTURE_2D, gBuffer->getNormalTexture());
				break;
			case 3:
				glBindTexture(GL_TEXTURE_2D, gBuffer->getSpecularTexture());
				break;
			case 4:
				break;
			}

			debugShader->setUniform1i("gTexture", 0);

			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			glEnable(GL_DEPTH_TEST);

		}
		else if (debugMode == 4)
		{

			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			for (int i = 0; i < lights.size(); i++)
			{
				lightingShader->bind();

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gBuffer->getPositionTexture());
				lightingShader->setUniform1i("gPosition", 0);

				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, gBuffer->getNormalTexture());
				lightingShader->setUniform1i("gNormal", 1);

				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, gBuffer->getAlbedoTexture());
				lightingShader->setUniform1i("gAlbedo", 2);

				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, gBuffer->getSpecularTexture());
				lightingShader->setUniform1i("gSpecular", 3);

				vec3 lp = lights[i].getPointLightPos();
				vec3 lc = lights[i].getPointLightColor();
				vec3 cp = cam.getPosition();
				lightingShader->setUniform4fv("lightPos", vec4(lp.x, lp.y, lp.z, 1.0f), lightingShader->getProgram());
				lightingShader->setUniform4fv("lightColor", vec4(lc.x, lc.y, lc.z, 1.0f), lightingShader->getProgram());
				lightingShader->setUniform1f("lightIntensity", lights[i].getPointLightIntensity());
				lightingShader->setUniform4fv("viewPos", vec4(cp.x, cp.y, cp.z, 1.0f), lightingShader->getProgram());
				lightingShader->setUniform1f("shininess", 32.0f);

				glBindVertexArray(quadVAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);

				
			}

			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);
		}

		glEnable(GL_DEPTH_TEST);

		for (int i = 0; i < lights.size(); i++)
		{
			lightNodes[i]->drawDebugLights(cam);
		}
		this->window->SwapBuffers();

#ifdef CI_TEST
		// if we're running CI, we want to return and exit the application after one frame
		// break the loop and hopefully exit gracefully
		break;
#endif
	}
}

} // namespace Example