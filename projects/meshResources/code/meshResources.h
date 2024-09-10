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

		/// open app
		bool Open();
		/// close app
		void Close();
		/// run app
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