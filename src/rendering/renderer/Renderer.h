#pragma once

#include <GLFW/glfw3.h>

class Renderer {
public:
	Renderer();
	~Renderer();

	void setupRender(GLFWwindow* window);
	void render(GLFWwindow* window);
};

