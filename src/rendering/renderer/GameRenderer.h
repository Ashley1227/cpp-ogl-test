#pragma once
#include "Renderer3D.h"

#include <vector>

#include "../shader/Shader.h"
#include "../camera/Camera.h"
#include "../VertexArray.h"

class GameRenderer : public Renderer3D {
private:
	Shader shader;
	unsigned int mvpLocation;

	unsigned int vao, ebo, vbo;
	unsigned int texture;

	VertexArray vertexArray;
	glm::vec3* cubePositions;

	int oldWindowWidth = INT32_MIN;
	int oldWindowHeight = INT32_MIN;
	int windowWidth;
	int windowHeight;
public:
	Camera camera;

	GameRenderer();
	~GameRenderer();

	void setupRender(GLFWwindow* window);
	void render(GLFWwindow* window);
};

