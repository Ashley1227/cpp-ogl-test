#include "GameRenderer.h"
#include "../../world/chunk/Chunk.h"

#include <stb_image.h>

unsigned int timeLocation;

Chunk chunk;

GameRenderer::GameRenderer() :
		Renderer3D(),
		shader("../res/shaders/texture.vert", "../res/shaders/texture.frag"),
		mvpLocation(glGetUniformLocation(shader.ID, "mvp")),
		vertexArray(180 * sizeof(float), new float[]{
				-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
				0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
				0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
				0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
				-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

				-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
				0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
				0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
				0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
				-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
				-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

				-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
				-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
				-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
				-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
				-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

				0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
				0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
				0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
				0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
				0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
				0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

				-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
				0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
				0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
				0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
				-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
				-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

				-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
				0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
				0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
				0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
				-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
				-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
		}),
		cubePositions(new glm::vec3[]{
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(2.0f, 5.0f, -15.0f),
				glm::vec3(-1.5f, -2.2f, -2.5f),
				glm::vec3(-3.8f, -2.0f, -12.3f),
				glm::vec3(2.4f, -0.4f, -3.5f),
				glm::vec3(-1.7f, 3.0f, -7.5f),
				glm::vec3(1.3f, -2.0f, -2.5f),
				glm::vec3(1.5f, 2.0f, -2.5f),
				glm::vec3(1.5f, 0.2f, -1.5f),
				glm::vec3(-1.3f, 1.0f, -1.5f)
		}),
		camera() {}

GameRenderer::~GameRenderer() {
	delete[] cubePositions;
}

void GameRenderer::setupRender(GLFWwindow* window) {
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.1f, 0.12f, 0.12f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Make VBO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// Make EBO
	glGenBuffers(1, &ebo);
	// Make VAO
	glGenVertexArrays(1, &vao);

	// set up drawing stuff
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	// coords
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	// Tex coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// store vertexArray in OpenGL's array buffer
//	glBufferData(GL_ARRAY_BUFFER, vertexArray.size, vertexArray, GL_STATIC_DRAW);
	chunk.generate();
	chunk.genMesh();
	glBufferData(GL_ARRAY_BUFFER, chunk.mesh.vertexArray.size, chunk.mesh.vertexArray, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunk.mesh.indexArray.size, chunk.mesh.indexArray, GL_STATIC_DRAW);

	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("../res/textures/container.jpg", &width, &height, &nrChannels, 0);

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	shader.use();

	view = camera.getView();
	timeLocation = glGetUniformLocation(shader.ID, "time");
}

void GameRenderer::render(GLFWwindow* window) {
//	double time1 = glfwGetTime();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(vao);

	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	if (windowWidth != oldWindowWidth || windowHeight != oldWindowHeight) {
		projection = glm::perspective(glm::radians(100.0f), (float) windowWidth / windowHeight, 0.1f, 100.0f);
		oldWindowWidth = windowWidth;
		oldWindowHeight = windowHeight;
	}
	view = camera.getView();

	auto vp = projection * view;
//	for (unsigned int i = 0; i < 10; i++) {
//		model = glm::mat4(1.0f);
//		model = glm::translate(model, cubePositions[i]);
////		float angle = 20.0f * i + 50.0f * (float) glfwGetTime();
////		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
//
//		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(vp * model));
//		glUniform1f(timeLocation, glfwGetTime());
//
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//	}
	model = glm::mat4(1.0f);
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(vp * model));
	glUniform1f(timeLocation, glfwGetTime());

	glDrawArrays(GL_TRIANGLES, 0, chunk.mesh.vertexArray.size);
//	glDrawElements(GL_TRIANGLES, chunk.mesh.indexArray.size, GL_UNSIGNED_INT, 0);
}