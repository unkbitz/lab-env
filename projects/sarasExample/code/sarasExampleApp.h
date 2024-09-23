#pragma once
#include "core/app.h"
#include "render/window.h"
#include "render/graphics/graphics.h"

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
		std::shared_ptr<MeshResource> cubeMesh;
		std::shared_ptr<GraphicsNode> cubeNode;
		//GraphicsNode graphicsNode;
		Camera cam;
		TextureResource texture;
		ShaderResource shader;
		Display::Window* window;
		MeshResource cube;
		MeshResource cube2;
	};
} // namespace Example