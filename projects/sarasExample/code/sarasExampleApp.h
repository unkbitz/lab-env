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
		std::shared_ptr<GraphicsNode> deerNode;
		Render::Grid* grid;
		Camera cam;
		Display::Window* window;
		vec3 camPosition = vec3(4.0f, 4.0f, 4.0f);
		vec3 camRotation = vec3(0.0f, 0.0f, 0.0f);
		vec3 forward;
		float moveSpeed = 0.05f;
		bool mouseLeftHeld = false;
		bool mouseRightHeld = false;
		double lastMouseX = 0.0, lastMouseY = 0.0;
	};
} // namespace Example