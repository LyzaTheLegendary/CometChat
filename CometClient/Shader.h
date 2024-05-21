#pragma once
#include <glad/glad.h>
#include <vector>
#include <stdint.h>
#include <string>

enum ShaderType : GLuint {
	Vertex = GL_VERTEX_SHADER
};

typedef GLuint GLshader;


struct Shader
{
public:
	Shader(ShaderType shaderType);
	~Shader();
	bool Compile(std::string shaderCode);
	GLshader GetPointer();
	std::string GetError();
private:
	GLshader m_shaderPointer = NULL;
	ShaderType m_shaderType;

	std::string m_errString;
};

