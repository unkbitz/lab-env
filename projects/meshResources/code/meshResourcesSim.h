#pragma once
#include "core/app.h"
#include "render/window.h"
#include "render/shaders/shaderResource.h"

namespace Mesh {
	class MeshResources : public Core::App {
	public:
		/// constructor
		MeshResources();
		/// destructor
		~MeshResources();

		bool Open();
		void Close();
		void Run();
		void CleanUp();
		
	private:
		//GLuint program;
		GLuint vertexShader;
		GLuint pixelShader;
		GLuint triangle;
		GLuint ibo;
		ShaderResource shader;
		Display::Window* window;
	};
} // namespace Mesh