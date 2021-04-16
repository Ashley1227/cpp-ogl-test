#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Uniform.h"

class Shader {
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	Uniform getUniform(const char* name) const;
	void use() const;

	void setMatrix4f(int uniform, glm::mat4 value) const;
	void set1f(int uniform, float value) const;
};