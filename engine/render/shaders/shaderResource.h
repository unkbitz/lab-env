#pragma once
#include "render/Grid.h"
#include <unordered_map>
#include "math/mat4.h"

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class ShaderResource {
public:
	ShaderResource();
	~ShaderResource();

	GLint getProgram();
	void load(const std::string& filepath);
	void bind() const;
	void unbind() const;
	GLint GetUniformLocation(const std::string& name);
	void setUniform4fv(char* name, vec4 const& vector, GLuint program);
	void setUnifrom4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniformMat4(char* name, mat4 const& matrix, GLuint program);
	void setUniform3f(const std::string& name, float v0, float v1, float v2);
	void setUniform1f(const std::string& name, float v0);
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

	unsigned int compileShader(unsigned int type, const std::string& source);
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
	ShaderProgramSource parseShader(const std::string& path);
	bool fileExists(const std::string& path);
};