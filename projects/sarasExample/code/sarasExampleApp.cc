#include "config.h"
#include "sarasExampleApp.h"
#include "math/mat4.h"
#include <random>

using namespace Display;
namespace Example
{
ExampleApp::ExampleApp() {}

ExampleApp::~ExampleApp() {}

void ExampleApp::SpawnPointLight(int i)
{
	pointLightVolumeShader->bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBuffer->getPositionTexture());
	pointLightVolumeShader->setUniform1i("gPosition", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gBuffer->getNormalTexture());
	pointLightVolumeShader->setUniform1i("gNormal", 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gBuffer->getAlbedoTexture());
	pointLightVolumeShader->setUniform1i("gAlbedo", 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gBuffer->getSpecularTexture());
	pointLightVolumeShader->setUniform1i("gSpecular", 3);

	vec3 lp = pointLights[i].getPointLightPos();
	vec3 lc = pointLights[i].getPointLightColor();
	float intensity = pointLights[i].getPointLightIntensity();
	float radius = pointLights[i].getPointLightRadius();
	vec3 cp = cam.getPosition();

	pointLightVolumeShader->setUniform3f("lightPos", lp.x, lp.y, lp.z);
	pointLightVolumeShader->setUniform3f("lightColor", lc.x, lc.y, lc.z);
	pointLightVolumeShader->setUniform1f("lightIntensity", intensity);
	pointLightVolumeShader->setUniform1f("lightRadius", radius);
	pointLightVolumeShader->setUniform3f("viewPos", cp.x, cp.y, cp.z);
	pointLightVolumeShader->setUniform1f("shininess", 8.0f);

	pointLightNodes[i]->drawLightVolume(cam, (float)width, (float)height, aspect);

	pointLightVolumeShader->unbind();
}

void ExampleApp::SpawnDirectionalLight()
{
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);

	directionalLightShader->bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBuffer->getPositionTexture());
	directionalLightShader->setUniform1i("gPosition", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gBuffer->getNormalTexture());
	directionalLightShader->setUniform1i("gNormal", 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gBuffer->getAlbedoTexture());
	directionalLightShader->setUniform1i("gAlbedo", 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gBuffer->getSpecularTexture());
	directionalLightShader->setUniform1i("gSpecular", 3);

	vec3 sunDir = sunLight.getDirectionalLightDir();
	vec3 sunColor = sunLight.getDirectionalLightColor();
	float sunIntensity = sunLight.getDirectionalLightIntensity();

	directionalLightShader->setUniform3f("lightDir", sunDir.x, sunDir.y, sunDir.z);
	directionalLightShader->setUniform3f("lightColor", sunColor.x, sunColor.y, sunColor.z);
	directionalLightShader->setUniform1f("lightIntensity", sunIntensity);

	vec3 cp = cam.getPosition();
	directionalLightShader->setUniform3f("viewPos", cp.x, cp.y, cp.z);
	directionalLightShader->setUniform1f("shininess", 32.0f);

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	directionalLightShader->unbind();
}

void ExampleApp::CreateShaders()
{
	// shader used for all objects/meshes
	geometryShader = std::make_shared<ShaderResource>();
	shaderPathGeometry = "assets/deferred_geometry.shader";
	geometryShader->load(shaderPathGeometry);

	// shader used for debug spheres representing point light positions
	lightDebugShader = std::make_shared<ShaderResource>();
	lightDebugShader->load("assets/light_debug.shader");

	//Light volume shader
	pointLightVolumeShader = std::make_shared<ShaderResource>();
	pointLightVolumeShader->load("assets/point_lighting.shader");

	// directional light shader
	directionalLightShader = std::make_shared<ShaderResource>();
	directionalLightShader->load("assets/directional_lighting.shader");
}

void ExampleApp::CreateSceneNodes()
{
	// Creating a GraphicsNodes to manage the meshes
	bunnyNode = std::make_shared<GraphicsNode>();
	cubeNode = std::make_shared<GraphicsNode>();
	horseNode = std::make_shared<GraphicsNode>();
	truckNode = std::make_shared<GraphicsNode>();
	ambulanceNode = std::make_shared<GraphicsNode>();
	raceNode = std::make_shared<GraphicsNode>();
	taxiNode = std::make_shared<GraphicsNode>();
	deliveryNode = std::make_shared<GraphicsNode>();
	fireTruckNode = std::make_shared<GraphicsNode>();
}

bool ExampleApp::LoadOBJMeshes()
{
	bunnyMesh = MeshResource::loadFromOBJ("assets/bunny.obj");
	if (!bunnyMesh)
	{
		std::cerr << "Failed to load bunny from OBJ" << std::endl;
		return false;
	}
	else
	{
		std::cout << "Bunny loaded from OBJ" << std::endl;
	}

	truckMesh = MeshResource::loadFromOBJ("assets/kenney_car-kit/Models/OBJ format/truck.obj");
	if (!truckMesh)
	{
		std::cerr << "Failed to load truck from OBJ" << std::endl;
		return false;
	}
	else
	{
		std::cout << "truckMesh loaded from OBJ" << std::endl;
	}

	ambulanceMesh = MeshResource::loadFromOBJ("assets/kenney_car-kit/Models/OBJ format/ambulance.obj");
	if (!ambulanceMesh)
	{
		std::cerr << "Failed to load ambulanceMesh from OBJ" << std::endl;
		return false;
	}
	else
	{
		std::cout << "ambulanceMesh loaded from OBJ" << std::endl;
	}

	deliveryMesh = MeshResource::loadFromOBJ("assets/kenney_car-kit/Models/OBJ format/delivery.obj");
	if (!deliveryMesh)
	{
		std::cerr << "Failed to load deliveryMesh from OBJ" << std::endl;
		return false;
	}
	else
	{
		std::cout << "deliveryMesh loaded from OBJ" << std::endl;
	}

	fireTruckMesh = MeshResource::loadFromOBJ("assets/kenney_car-kit/Models/OBJ format/firetruck.obj");
	if (!fireTruckMesh)
	{
		std::cerr << "Failed to load fireTruckMesh from OBJ" << std::endl;
		return false;
	}
	else
	{
		std::cout << "fireTruckMesh loaded from OBJ" << std::endl;
	}

	raceMesh = MeshResource::loadFromOBJ("assets/kenney_car-kit/Models/OBJ format/race.obj");
	if (!raceMesh)
	{
		std::cerr << "Failed to load race from OBJ" << std::endl;
		return false;
	}
	else
	{
		std::cout << "raceMesh loaded from OBJ" << std::endl;
	}

	taxiMesh = MeshResource::loadFromOBJ("assets/kenney_car-kit/Models/OBJ format/taxi.obj");
	if (!taxiMesh)
	{
		std::cerr << "Failed to load taxiMesh from OBJ" << std::endl;
		return false;
	}
	else
	{
		std::cout << "taxiMesh loaded from OBJ" << std::endl;
	}

	cubeMesh = MeshResource::createCube(0.5f, 0.5f, 0.5f);
	if (!cubeMesh)
	{
		std::cerr << "Failed to create cube mesh" << std::endl;
		return false;
	}
	else
	{
		std::cout << "Cube created" << std::endl;
	}

	horseMesh = MeshResource::loadFromOBJ("assets/horse_conv.obj");
	if (!horseMesh)
	{
		std::cerr << "Failed to load horse from OBJ" << std::endl;
		return false;
	}
	else
	{
		std::cout << "Horse loaded from OBJ" << std::endl;
	}

	lightMesh = MeshResource::loadFromOBJ("assets/tinker.obj");
	if (!lightMesh)
	{
		std::cerr << "Failed to load lightMesh from OBJ" << std::endl;
		return false;
	}
	else
	{
		std::cout << "Light Mesh loaded from OBJ" << std::endl;
	}
	return true;
}

void ExampleApp::CreateMaterials()
{
	woodMaterial = std::make_shared<BlinnPhongMaterial>(geometryShader, 16.0f);
	plasticMaterial = std::make_shared<BlinnPhongMaterial>(geometryShader, 32.0f);
	HorseMaterial = std::make_shared<BlinnPhongMaterial>(geometryShader, 1.0f);
	lampMaterial = std::make_shared<BlinnPhongMaterial>(lightDebugShader, 64.0f);
	carMaterial = std::make_shared<BlinnPhongMaterial>(geometryShader, 32.0f);

	// Loading texture
	std::shared_ptr<TextureResource> woodTex = std::make_shared<TextureResource>();
	std::shared_ptr<TextureResource> woodSpecTex = std::make_shared<TextureResource>();
	std::shared_ptr<TextureResource> rubikTex = std::make_shared<TextureResource>();
	std::shared_ptr<TextureResource> rubikSpecTex = std::make_shared<TextureResource>();
	std::shared_ptr<TextureResource> horseTex = std::make_shared<TextureResource>();
	std::shared_ptr<TextureResource> horseSpecTex = std::make_shared<TextureResource>();

	woodTex->loadTextureURI("assets/wood.jpg", 0);
	woodSpecTex->loadTextureURI("assets/wood_spec.jpg", 0);
	rubikTex->loadTextureURI("assets/Rubik2.png", 0);
	rubikSpecTex->loadTextureURI("assets/Rubik2_spec.png", 0);
	horseTex->loadTextureURI("assets/horse.jpg", 1);
	horseSpecTex->loadTextureURI("assets/horse_spec2.jpg", 1);

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
}

void ExampleApp::CreatePointLights()
{
	for (int i = 0; i < 20; i++)
	{
		Lighting light;

		float r = (rand() % 100) / 100.0f;
		float g = (rand() % 100) / 100.0f;
		float b = (rand() % 100) / 100.0f;

		std::mt19937 rng(std::random_device{}());
		std::uniform_real_distribution<float> distXZ(-5.0f, 5.0f);
		std::uniform_real_distribution<float> distY(0.5f, 3.5f);

		float x = distXZ(rng);
		float y = distY(rng);
		float z = distXZ(rng);

		std::shared_ptr<GraphicsNode> debugLightNode = std::make_shared<GraphicsNode>();
		std::shared_ptr<GraphicsNode> pointLightNode = std::make_shared<GraphicsNode>();

		light.setColor(vec3(r, g, b));
		light.setPosition(vec3(x, y, z));

		float radius = light.getPointLightRadius();

		// small debug sphere
		debugLightNode->setMesh(lightMesh);
		debugLightNode->setShader(lightDebugShader);
		debugLightNode->setMaterial(lampMaterial);
		debugLightNode->setScale(vec3(0.05f, 0.05f, 0.05f));
		debugLightNode->setBaseColor(vec4(r, g, b, 1.0f));
		debugLightNode->setPosition(vec4(x, y, z, 1.0f));

		// bigger light volume sphere
		pointLightNode->setMesh(lightMesh);
		pointLightNode->setShader(pointLightVolumeShader);
		pointLightNode->setMaterial(lampMaterial);
		pointLightNode->setScale(vec3(radius * 1.15f, radius * 1.15f, radius * 1.15f));
		pointLightNode->setBaseColor(vec4(r, g, b, 1.0f));

		pointLights.push_back(light);
		debugLightNodes.push_back(debugLightNode);
		pointLightNodes.push_back(pointLightNode);

		vec3 basePos(x, y, z);
		light.setPosition(basePos);
		lightBasePos.push_back(basePos);
	}
}

// full screen quad used for directional light
void ExampleApp::CreateFullScreenQuad()
{
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
}

void ExampleApp::BindOBJNodes()
{
	bunnyNode->setMesh(bunnyMesh);
	bunnyNode->setShader(geometryShader);
	bunnyNode->setMaterial(woodMaterial);

	cubeNode->setMesh(cubeMesh);
	cubeNode->setShader(geometryShader);
	cubeNode->setMaterial(plasticMaterial);

	horseNode->setMesh(horseMesh);
	horseNode->setShader(geometryShader);
	horseNode->setMaterial(HorseMaterial);

	truckNode->setMesh(truckMesh);
	truckNode->setShader(geometryShader);
	truckNode->setMaterial(carMaterial);

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
}

bool ExampleApp::setUpOBJObjects()
{
	if (!LoadOBJMeshes())
	{
		return false;
	}
	
	CreateMaterials();
	BindOBJNodes();
	CreatePointLights();
	return true;
}

void ExampleApp::LoadGLTFObjects()
{
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
}

void ExampleApp::SetUpScene()
{
	float degrees360 = 3.14f * 2.0f;

	mat4 horseRotationMatrix = rotationaxis(vec3(1.0f, 0.0f, 0.0f), -degrees360 * 0.25f);
	mat4 avocadoRotationMatrix =
		rotationaxis(vec3(0.0f, 1.0f, 0.0f), degrees360 * 0.4f) *
		rotationaxis(vec3(1.0f, 0.0f, 0.0f), degrees360 * 0.25f);
	mat4 dhemletRotationMatrix =
		rotationaxis(vec3(1.0f, 0.0f, 0.0f), degrees360 * 0.245f);

	mat4 identotyMatrix;
	mat4 carMatrix = rotationaxis(vec3(0.0f, 1.0f, 0.0f), -degrees360 * 0.125f);

	bunnyNode->setScale(vec3(0.25f, 0.25f, 0.25f));
	bunnyNode->setRotation(identotyMatrix);
	bunnyNode->setPosition(vec4(0.5f, -0.006f, -0.5f, 1.0f));

	cubeNode->setScale(vec3(0.5f, 0.5f, 0.5f));
	cubeNode->setRotation(identotyMatrix);
	cubeNode->setPosition(vec4(0.0f, 0.125f, 0.0f, 1.0f));

	horseNode->setScale(vec3(0.05f, 0.05f, 0.05f));
	horseNode->setRotation(horseRotationMatrix);
	horseNode->setPosition(vec4(-0.5f, 0.0f, -0.5f, 1.0f));

	truckNode->setScale(vec3(0.5f, 0.5f, 0.5f));
	truckNode->setRotation(carMatrix);
	truckNode->setPosition(vec4(4.5f, 0.0f, 2.0f, 1.0f));
	truckNode->setUseDiffuseTexture(false);
	truckNode->setBaseColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));

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
	fireTruckNode->setPosition(vec4(1.5f, 0.0f, 2.0f, 1.0f));
	fireTruckNode->setUseDiffuseTexture(false);
	fireTruckNode->setBaseColor(vec4(0.8f, 0.0f, 0.0f, 1.0f));

	deliveryNode->setScale(vec3(0.5f, 0.5f, 0.5f));
	deliveryNode->setRotation(carMatrix);
	deliveryNode->setPosition(vec4(3.0f, 0.0f, 2.0f, 1.0f));
	deliveryNode->setUseDiffuseTexture(false);
	deliveryNode->setBaseColor(vec4(0.0f, 0.7f, 0.5f, 1.0f));

	GLTFCubeNode->setScale(vec3(0.125f, 0.125f, 0.125f));
	GLTFCubeNode->setRotation(identotyMatrix);
	GLTFCubeNode->setPosition(vec4(-0.5f, 0.1245f, 0.5f, 1.0f));

	avocadoNode->setScale(vec3(5.0f, 5.0f, 5.0f));
	avocadoNode->setRotation(avocadoRotationMatrix);
	avocadoNode->setPosition(vec4(0.0f, 0.07f, 0.5f, 1.0f));

	damagedHelmetNode->setScale(vec3(0.2f, 0.2f, 0.2f));
	damagedHelmetNode->setRotation(dhemletRotationMatrix);
	damagedHelmetNode->setPosition(vec4(0.5f, 0.17f, 0.5f, 1.0f));

	rubberDuckNode->setScale(vec3(20.0f, 20.0f, 20.0f));
	rubberDuckNode->setRotation(identotyMatrix);
	rubberDuckNode->setPosition(vec4(-5.0f, 0.0f, 0.0f, 1.0f));

	horseHeadNode->setScale(vec3(20.0f, 20.0f, 20.0f));
	horseHeadNode->setRotation(identotyMatrix);
	horseHeadNode->setPosition(vec4(5.0f, 0.0f, -5.0f, 1.0f));

	lionHeadNode->setScale(vec3(20.0f, 20.0f, 20.0f));
	lionHeadNode->setRotation(identotyMatrix);
	lionHeadNode->setPosition(vec4(0.0f, 0.0f, -5.0f, 1.0f));

	elephantNode->setScale(vec3(30.0f, 30.0f, 30.0f));
	elephantNode->setRotation(identotyMatrix);
	elephantNode->setPosition(vec4(5.0f, 0.0f, 0.0f, 1.0f));

	flightHelmetNode->setScale(vec3(1.0f, 1.0f, 1.0f));
	flightHelmetNode->setRotation(identotyMatrix);
	flightHelmetNode->setPosition(vec4(1.0f, 0.0f, 0.5f, 1.0f));

	kiwiNode->setScale(vec3(1.0f, 1.0f, 1.0f));
	kiwiNode->setRotation(identotyMatrix);
	kiwiNode->setPosition(vec4(2.0f, 0.0f, 0.5f, 1.0f));

	pomegranateNode->setScale(vec3(1.0f, 1.0f, 1.0f));
	pomegranateNode->setRotation(identotyMatrix);
	pomegranateNode->setPosition(vec4(2.0f, 0.0f, 0.0f, 1.0f));

	appleNode->setScale(vec3(1.0f, 1.0f, 1.0f));
	appleNode->setRotation(identotyMatrix);
	appleNode->setPosition(vec4(2.0f, 0.0f, -0.5f, 1.0f));
}

void ExampleApp::DrawObjects()
{
	bunnyNode->drawGeometry(cam, aspect);
	cubeNode->drawGeometry(cam, aspect);
	horseNode->drawGeometry(cam, aspect);
	truckNode->drawGeometry(cam, aspect);
	ambulanceNode->drawGeometry(cam, aspect);
	raceNode->drawGeometry(cam, aspect);
	taxiNode->drawGeometry(cam, aspect);
	deliveryNode->drawGeometry(cam, aspect);
	fireTruckNode->drawGeometry(cam, aspect);
	GLTFCubeNode->drawGeometry(cam, aspect);
	avocadoNode->drawGeometry(cam, aspect);
	damagedHelmetNode->drawGeometry(cam, aspect);
	rubberDuckNode->drawGeometry(cam, aspect);
	horseHeadNode->drawGeometry(cam, aspect);
	lionHeadNode->drawGeometry(cam, aspect);
	elephantNode->drawGeometry(cam, aspect);
	flightHelmetNode->drawGeometry(cam, aspect);
	kiwiNode->drawGeometry(cam, aspect);
	pomegranateNode->drawGeometry(cam, aspect);
	appleNode->drawGeometry(cam, aspect);
}

bool ExampleApp::Open()
{
	App::Open();
	this->window = new Display::Window;

	if (this->window->Open())
	{
		this->window->GetSize(width, height);
		aspect = static_cast<float>(width) / static_cast<float>(height);
		this->gBuffer = std::make_unique<GBuffer>();

		if (!this->gBuffer->initialize(width, height))
		{
			std::cerr << "Failed to initialize GBuffer" << std::endl;
			return false;
		}

		sunLight.setDirektionalLightDir(vec3(-0.5f, -1.0f, -0.3f));
		sunLight.setDirektionalLightColor(vec3(1.0f, 1.0f, 0.9f));
		sunLight.setDirektionalLightIntensity(0.1f);

		CreateFullScreenQuad();
		CreateShaders();
		CreateSceneNodes();

		if (!setUpOBJObjects())
		{
			return false;
		}

		LoadGLTFObjects();
		  
		glClearColor(0.02f, 0.02f, 0.02f, 1.0f);
		SetUpScene();

		window->SetKeyPressFunction([this](int key, int scancode, int action, int mods)
			{
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			{
				this->window->Close();
			}
			if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
			{
				if (!lightPause)
				{
					lightPause = true;
					std :: cout << "Light movement paused" << std::endl;
				}
				else if (lightPause)
				{
					lightPause = false;
					std::cout << "Light movement restarted" << std::endl;
				}
			}

			vec3 camMovement(0, 0, 0);
			
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
			{
				switch (key)
				{
				case GLFW_KEY_W: camMovement += cam.getFront() * moveSpeed; break;
				case GLFW_KEY_S: camMovement -= cam.getFront() * moveSpeed; break;
				case GLFW_KEY_A: camMovement -= normalize(cross(cam.getFront(), cam.getUp())) * moveSpeed; break;
				case GLFW_KEY_D: camMovement += normalize(cross(cam.getFront(), cam.getUp())) * moveSpeed; break;
				}
			}
			cam.setPosition(cam.getPosition() + camMovement);
			cam.updateCameraVectors();
		});

		window->SetMouseMoveFunction([this](float xpos, float ypos)
			{
				if (mouseLeftHeld)
				{
					if (firstMouse)
					{
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
				else
				{
					// Reset firstMouse when the mouse button is released
					firstMouse = true;
				}
		});

		window->SetMousePressFunction([this](int button, int action, int mods)
			{
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
			{
				mouseLeftHeld = true;
			}
			else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
			{
				mouseLeftHeld = false;
			}
		});
		return true;
	}
	return false;
}
 
void ExampleApp::Close()
{
	if (this->window->IsOpen())
	{
		this->window->Close();
	}
	if (quadVBO != 0)
	{
		glDeleteBuffers(1, &quadVBO);
		quadVBO = 0;
	}
	if (quadVAO != 0)
	{
		glDeleteVertexArrays(1, &quadVAO);
		quadVAO = 0;
	}
	Core::App::Close();
}

void ExampleApp::Run()
{
	glEnable(GL_DEPTH_TEST);
	float elapsedTime = 0.0f;
	float pauseTime = 0.0f;
	float lastPauseStart = 0.0f;
	bool isPaused = false;
	
	float initialTime = float(glfwGetTime());
	while (this->window->IsOpen())
	{
		this->window->Update();

		gBuffer->bindForWriting();
		glViewport(0, 0, width, height);

		const float zero4[4] = { 0.f, 0.f, 0.f, 0.f };
		const float one = 1.f;

		glClearBufferfv(GL_COLOR, 0, zero4);
		glClearBufferfv(GL_COLOR, 1, zero4);
		glClearBufferfv(GL_COLOR, 2, zero4);
		glClearBufferfv(GL_COLOR, 3, zero4);
		glClearBufferfv(GL_DEPTH, 0, &one);

		glEnable(GL_DEPTH_TEST);

		float currentTime = float(glfwGetTime());

		for (int i = 0; i < pointLights.size(); i++)
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
				vec3 base = lightBasePos[i];

				elapsedTime = float(glfwGetTime()) - initialTime - pauseTime;
				float angle = 0.05 * elapsedTime * (0.3f + 0.5f * i) + i * 0.05f;
				float orbitRadius = 2.0f + 0.2f * i;
				float offsetX = cosf(angle) * orbitRadius;
				float offsetZ = sinf(angle) * orbitRadius;
				float offsetY = 1.0f + 0.3f * sinf(angle * 1.7f);

				vec4 newLightPos(base.x + offsetX, base.y + offsetY, base.z + offsetZ, 1.0f);

				pointLights[i].setPosition(vec3(newLightPos.x, newLightPos.y, newLightPos.z));
				debugLightNodes[i]->setPosition(newLightPos);
				pointLightNodes[i]->setPosition(newLightPos);
			}
		}

		DrawObjects();

		gBuffer->unbind();

		glClear(GL_COLOR_BUFFER_BIT);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer->getFramebuffer());
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(
			0, 0, width, height,
			0, 0, width, height,
			GL_DEPTH_BUFFER_BIT,
			GL_NEAREST
		);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		SpawnDirectionalLight();

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glDepthFunc(GL_GEQUAL);
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glCullFace(GL_FRONT);

		for (int i = 0; i < pointLights.size(); i++)
		{
			// Frustum Culling
			if (!cam.sphereInFrustum(pointLights[i].getPointLightPos(),	pointLights[i].getPointLightRadius(), aspect))
			{
				continue;
			}
			SpawnPointLight(i);
		}

		glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);

		for (int i = 0; i < debugLightNodes.size(); i++)
		{
			debugLightNodes[i]->drawDebugLights(cam, aspect);
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