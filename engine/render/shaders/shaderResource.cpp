#pragma once
#include "config.h"
#include "shaderResource.h"
#include <iostream>
#include <fstream>
#include <sstream>

ShaderResource::ShaderResource()
	: m_RendererID(0) {

}

//ShaderResource::ShaderResource(const std::string& filepath)
//	: m_FilePath(filepath), m_RendererID(0)
//{
//	ShaderProgramSource source = ParseShader(filepath);
//	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
//	 
//}
ShaderResource::~ShaderResource() {
	glDeleteProgram(m_RendererID);
}

bool ShaderResource::fileExists(const std::string& path) {
	std::ifstream file(path);
	return file.good();
}

unsigned int ShaderResource::compileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	GLint length= static_cast<GLint>(std::strlen(src));
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}
	return id;
}

ShaderProgramSource ShaderResource::parseShader(const std::string& path) {
	std::ifstream stream(path);
	if (!stream.is_open()) {
		std::cerr << "ERROR: Could not open shader file: " << path << std::endl;
		return { "", "" };
	}
	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}

unsigned int ShaderResource::createShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);
	
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

void ShaderResource::load(const std::string& filepath) {
	ShaderProgramSource source = parseShader(filepath);
	m_RendererID = createShader(source.VertexSource, source.FragmentSource);
}
void ShaderResource::bind() const {
	glUseProgram(m_RendererID);
}
void ShaderResource::unbind() const {
	glUseProgram(0);
}

void ShaderResource::setUniform4fv(char* name, vec4 const& vector, GLuint program) {
	glUniform4fv(GetUniformLocation(name), 1, &vector[0]);
}
void ShaderResource::setUnifrom4f(const std::string& name, float v0, float v1, float v2, float v3) {
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
	
}
void ShaderResource::setUniformMat4(char* name, mat4 const& matrix, GLuint program) {
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

GLint ShaderResource::GetUniformLocation(const std::string& name) {
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[name];
	}
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1) {
		std::cout << "Warning: uniform '" << name <<"' don't exsist." << endl;
	}
	m_UniformLocationCache[name] = location;
	return location;
}

GLint ShaderResource::getProgram() {
	return m_RendererID;
}