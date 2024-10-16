#pragma once
#include "core/app.h"
#include "render/window.h"
#include "render/graphics/graphics.h"
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
		std::shared_ptr<GraphicsNode> cubeNode; // update to vector
		std::shared_ptr<GraphicsNode> bunnyNode;
		std::shared_ptr<GraphicsNode> lightNode;
		std::shared_ptr<GraphicsNode> horseNode;
		
		Display::Window* window;
		Render::Grid* grid;
		
		Lighting light;
		Camera cam;
		
		vec3 forward;
		float moveSpeed = 0.05f;
		bool mouseLeftHeld = false;
		bool mouseRightHeld = false;
		bool firstMouse = true;
		bool lightPause = false;

		float lastMouseX = 0.0f;
		float lastMouseY = 0.0f;
	};
} // namespace Example