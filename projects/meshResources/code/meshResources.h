#pragma once
#include "core/app.h"
#include "render/window.h"

namespace Mesh
{
	class MeshResources : public Core::App
	{
	public:
		/// constructor
		MeshResources();
		/// destructor
		~MeshResources();

		bool Open();
		void Close();
		void Run();
		
	private:
		GLuint program;
		GLuint vertexShader;
		GLuint pixelShader;
		GLuint triangle;
		GLuint ibo;
		Display::Window* window;
	};
} // namespace Mesh