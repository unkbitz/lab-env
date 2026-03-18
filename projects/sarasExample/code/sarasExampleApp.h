#pragma once
#include "core/app.h"
#include "render/window.h"
#include "render/meshes/GLTFLoader.h"
#include "render/graphics/gBuffer.h"

namespace Example
{
	class ExampleApp : public Core::App
	{
	public:
		/// constructor
		ExampleApp();
		/// destructor
		~ExampleApp();

		bool Open();
		void Close();
		void Run();
		void SpawnPointLight(int i);
		void SpawnDirectionalLight();
		void CreateShaders();
		void CreateSceneNodes();
		bool setUpOBJObjects();

		bool LoadOBJMeshes();
		void CreateMaterials();
		void CreatePointLights();
		void CreateFullScreenQuad();
		void BindOBJNodes();

		void LoadGLTFObjects();
		void SetUpScene();
		void DrawObjects();

	private:

		std::unique_ptr<GBuffer> gBuffer;
		std::shared_ptr<ShaderResource> geometryShader;
		std::shared_ptr<ShaderResource> lightDebugShader;
		std::shared_ptr<ShaderResource> directionalLightShader;
		std::shared_ptr<ShaderResource> pointLightVolumeShader;
		std::string shaderPathGeometry;

		// nodes obj-objects
		std::shared_ptr<GraphicsNode> cubeNode;
		std::shared_ptr<GraphicsNode> bunnyNode;
		std::shared_ptr<GraphicsNode> horseNode;
		std::shared_ptr<GraphicsNode> truckNode;
		std::shared_ptr<GraphicsNode> ambulanceNode;
		std::shared_ptr<GraphicsNode> raceNode;
		std::shared_ptr<GraphicsNode> taxiNode;
		std::shared_ptr<GraphicsNode> deliveryNode;
		std::shared_ptr<GraphicsNode> fireTruckNode;

		// nodes gltf-objects
		std::shared_ptr<GraphicsNode> GLTFCubeNode;
		std::shared_ptr<GraphicsNode> avocadoNode;
		std::shared_ptr<GraphicsNode> damagedHelmetNode;
		std::shared_ptr<GraphicsNode> flightHelmetNode;
		std::shared_ptr<GraphicsNode> rubberDuckNode;
		std::shared_ptr<GraphicsNode> horseHeadNode;
		std::shared_ptr<GraphicsNode> lionHeadNode;
		std::shared_ptr<GraphicsNode> elephantNode;
		std::shared_ptr<GraphicsNode> appleNode;
		std::shared_ptr<GraphicsNode> kiwiNode;
		std::shared_ptr<GraphicsNode> pomegranateNode;

		// meshes
		std::shared_ptr<MeshResource> bunnyMesh;
		std::shared_ptr<MeshResource> truckMesh;
		std::shared_ptr<MeshResource> ambulanceMesh;
		std::shared_ptr<MeshResource> deliveryMesh;
		std::shared_ptr<MeshResource> fireTruckMesh;
		std::shared_ptr<MeshResource> raceMesh;
		std::shared_ptr<MeshResource> taxiMesh;
		std::shared_ptr<MeshResource> cubeMesh;
		std::shared_ptr<MeshResource> horseMesh;
		std::shared_ptr<MeshResource> lightMesh;

		// Materials
		std::shared_ptr<BlinnPhongMaterial> woodMaterial;
		std::shared_ptr<BlinnPhongMaterial> plasticMaterial;
		std::shared_ptr<BlinnPhongMaterial> HorseMaterial;
		std::shared_ptr<BlinnPhongMaterial> lampMaterial;
		std::shared_ptr<BlinnPhongMaterial> carMaterial;

		Display::Window* window;
		int32 width;
		int32 height;
		float aspect;
		Camera cam;

		// ligths
		std::vector<Lighting> pointLights;
		std::vector<std::shared_ptr<GraphicsNode> > pointLightNodes;
		std::vector<std::shared_ptr<GraphicsNode> > debugLightNodes;
		std::vector<vec3> lightBasePos;
		Lighting sunLight;

		float moveSpeed = 0.05f;
		bool mouseLeftHeld = false;
		bool firstMouse = true;
		bool lightPause = false;

		float lastMouseX = 0.0f;
		float lastMouseY = 0.0f;

		GLuint quadVAO = 0;
		GLuint quadVBO = 0;
	};
} // namespace Example