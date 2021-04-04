#define STB_IMAGE_IMPLEMENTATION
#include "rendering/includes.h"

#include <stb_image.h>

#include "rendering/renderer/GameRenderer.h"

GLFWwindow* WINDOW;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glfwMakeContextCurrent(window);
	glViewport(0, 0, width, height);
}

int main() {
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Make MacOS shut up

	WINDOW = glfwCreateWindow(640, 480, "OpenGL Window", NULL, NULL);
	if (!WINDOW) {
		fprintf(stderr, "Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(WINDOW);
	glfwSwapInterval(1);
	glfwSetFramebufferSizeCallback(WINDOW, framebufferSizeCallback);


	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}
	printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
	printf("Using OpenGL %s\n\n", glGetString(GL_VERSION));

	double p = glfwGetTime();
	double dt;

	GameRenderer renderer;

	renderer.setupRender(WINDOW);
	glfwSetFramebufferSizeCallback(WINDOW, framebufferSizeCallback);
	while (!glfwWindowShouldClose(WINDOW)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		dt = glfwGetTime() - p;
		p = glfwGetTime();

		renderer.camera.render();
		renderer.camera.processInput(WINDOW, dt);
		renderer.render(WINDOW);

		glfwSwapBuffers(WINDOW);
		glfwPollEvents();
	}

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);

	glfwTerminate();
	return 0;
}

