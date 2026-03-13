#pragma once
#include "core/app.h"
#include "render/window.h"
#include "render/meshes/GLTFLoader.h"
#include "render/graphics/gBuffer.h"
#include "Render/Grid.h"

namespace Example {
	class ExampleApp : public Core::App {
	public:
		/// constructor
		ExampleApp();
		/// destructor
		~ExampleApp();

		bool Open();
		void Close();
		void Run();
		void CleanUp();

	private:

		std::unique_ptr<GBuffer> gBuffer;
		std::shared_ptr<ShaderResource> geometryShader;
		std::shared_ptr<ShaderResource> lightDebugShader;
		std::shared_ptr<ShaderResource> debugShader;
		std::shared_ptr<ShaderResource> lightingShader;

		std::shared_ptr<GraphicsNode> cubeNode; // update to vector
		std::shared_ptr<GraphicsNode> bunnyNode;
		//std::shared_ptr<GraphicsNode> lightNode;
		std::shared_ptr<GraphicsNode> horseNode;
		std::shared_ptr<GraphicsNode> ambulanceNode;
		std::shared_ptr<GraphicsNode> raceNode;
		std::shared_ptr<GraphicsNode> taxiNode;
		std::shared_ptr<GraphicsNode> deliveryNode;
		std::shared_ptr<GraphicsNode> fireTruckNode;

		std::shared_ptr<GraphicsNode> GLTFCubeNode;
		std::shared_ptr<GraphicsNode> avocadoNode;
		std::shared_ptr<GraphicsNode> damagedHelmetNode;
		std::shared_ptr<GraphicsNode> flightHelmetNode;
		std::shared_ptr<GraphicsNode> normalTangentMirrorNode;
		std::shared_ptr<GraphicsNode> sponzaNode;
		std::shared_ptr<GraphicsNode> rubberDuckNode;
		std::shared_ptr<GraphicsNode> horseHeadNode;
		std::shared_ptr<GraphicsNode> lionHeadNode;
		std::shared_ptr<GraphicsNode> elephantNode;
		std::shared_ptr<GraphicsNode> appleNode;
		std::shared_ptr<GraphicsNode> kiwiNode;
		std::shared_ptr<GraphicsNode> pomegranateNode;

		Display::Window* window;
		Render::Grid* grid;

		std::vector<Lighting> lights;
		std::vector<std::shared_ptr<GraphicsNode> > lightNodes;
		//Lighting light;
		Camera cam;
		int seed = 42;

		vec3 forward;
		float moveSpeed = 0.05f;
		bool mouseLeftHeld = false;
		bool mouseRightHeld = false;
		bool firstMouse = true;
		bool lightPause = false;

		float lastMouseX = 0.0f;
		float lastMouseY = 0.0f;

		GLuint quadVAO = 0;
		GLuint quadVBO = 0;
	};
} // namespace Example