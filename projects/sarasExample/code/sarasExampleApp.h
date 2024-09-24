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
		std::shared_ptr<GraphicsNode> cubeNode;
		Render::Grid* grid;
		Camera cam;
		Display::Window* window;
		vec3 cubePosition = vec3(0.0f, 0.0f, -5.0f);
		vec3 cubeRotation = vec3(0.0f, 0.0f, 0.0f);
		float moveSpeed = 0.05f;
		bool mouseHeld = false;
		double lastMouseX = 0.0, lastMouseY = 0.0;
	};
} // namespace Example