#pragma once
#include "core/app.h"
#include "render/window.h"
#include "render/shaders/shaderResource.h"

namespace MeshResourceSim {
	class MeshResourceSimApp : public Core::App {
	public:
		/// constructor
		MeshResourceSimApp();
		/// destructor
		~MeshResourceSimApp();

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
} // namespace 