#pragma once
#include "core/app.h"
#include "render/window.h"
#include "render/shaders/shaderResource.h"
#include "render/camera/camera.h"
#include "render/textures/textureResource.h"
#include "render/meshes/meshResource.h"

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
		camera cam;
		TextureResource texture;
		ShaderResource shader;
		Display::Window* window;
		MeshResource cube;
		MeshResource cube2;
	};
} // namespace Example