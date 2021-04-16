#pragma once
#include "Renderer3D.h"

#include <vector>

#include "../shader/Shader.h"
#include "../camera/Camera.h"
#include "../VertexArray.h"
#include "./FPSCounterRenderer.h"

class GameRenderer : public Renderer3D, FPSCounterRenderer {
private:
	Shader shader;
	unsigned int mvpLocation;

	int oldWindowWidth = INT32_MIN;
	int oldWindowHeight = INT32_MIN;
	int windowWidth;
	int windowHeight;
public:

	GameRenderer();
	~GameRenderer();

	void setupRender(GLFWwindow* window);
	void render(GLFWwindow* window);
};

