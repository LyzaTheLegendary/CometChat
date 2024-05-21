#include "Shader.h"

Shader::Shader(ShaderType shaderType)
{
	m_shaderType = shaderType;
	m_shaderPointer = glCreateShader(shaderType);
	m_errString = "NOT COMPILED";
}

Shader::~Shader()
{
	if (m_shaderPointer == NULL) {
		return;
	}

	glDeleteShader(m_shaderPointer);
}

bool Shader::Compile(std::string shaderCode)
{
	if (m_shaderPointer == NULL)
		throw std::exception("Invalid shader pointer.");
	
	const char *rawShaderString = shaderCode.c_str();
	glShaderSource(m_shaderPointer, 1, &rawShaderString, NULL);
	glCompileShader(m_shaderPointer);

	int success;
	char infoLog[512];
	glGetShaderiv(m_shaderPointer, GL_COMPILE_STATUS, &success);

	infoLog[512] = NULL;
	if (!success)
	{
		m_errString = infoLog; // if not null terminated it will fail here?
		return false;
	}

	m_errString = "SUCCESS";
	return true;
}

GLshader Shader::GetPointer()
{
	return m_shaderPointer;
}

std::string Shader::GetError()
{
	return m_errString;
}

