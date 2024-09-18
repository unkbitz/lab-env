#pragma once
#include "render/Grid.h"
#include <unordered_map>
#include "math/mat4.h"

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class ShaderResource {
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	ShaderResource();
	~ShaderResource();

	GLint getProgram();
	void Load(const std::string& filepath);
	void Bind() const;
	void Unbind() const;
	GLint GetUniformLocation(const std::string& name);
	void setUniform4fv(char* name, vec4 const& vector, GLuint program);
	void setUnifrom4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniformMat4(char* name, mat4 const& matrix, GLuint program);
private:
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	ShaderProgramSource ParseShader(const std::string& path);
	bool FileExists(const std::string& path);
};