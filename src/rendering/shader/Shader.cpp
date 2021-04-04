#include "Shader.h"

#include <string>
#include <sstream>
#include <iostream>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	// 1. retrieve the vertex/fragment source code from filePath
	FILE* vShaderFile = fopen(vertexPath, "r");
	FILE* fShaderFile = fopen(fragmentPath, "r");

	fseek(vShaderFile, 0, SEEK_END);
	size_t vSize = ftell(vShaderFile);
	fseek(fShaderFile, 0, SEEK_END);
	size_t fSize = ftell(fShaderFile);

	// +1 for 0-termination
	char* vShaderCode = new char[vSize + 1];
	char* fShaderCode = new char[fSize + 1];

	rewind(vShaderFile);
	fread(vShaderCode, sizeof(char), vSize, vShaderFile);
	rewind(fShaderFile);
	fread(fShaderCode, sizeof(char), fSize, fShaderFile);

	vShaderCode[vSize] = 0;
	fShaderCode[fSize] = 0;

	// 2. compile shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];
	// vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "Vertex shader compilation failed :c\n" << infoLog << std::endl;
	};

	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "Fragment shader compilation failed :c\n" << infoLog << std::endl;
	};

	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// print linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "Shader program linking failed :c\n" << infoLog << std::endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	fclose(vShaderFile);
	fclose(fShaderFile);
	delete[] vShaderCode;
	delete[] fShaderCode;
}

void Shader::use() const {
	glUseProgram(ID);
}
Uniform Shader::getUniform(const char* name) const {
	return Uniform{glGetUniformLocation(ID, name)};
}

void Shader::setMatrix4f(int uniform, glm::mat4 value) const {
	glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::set1f(int uniform, float value) const {
	glUniform1f(uniform, value);
}