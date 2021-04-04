#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera {
private:
	glm::vec3 pos;

	double yaw, pitch;

	glm::vec3 direction;
	glm::vec3 up;
	glm::vec3 right;

	double lastX, lastY;
	double sensitivity;
	bool showCursor;
public:
	Camera();

	void render();
	void processInput(GLFWwindow* window, double dt);

	inline glm::vec3 calculateUp() const {
		return glm::normalize(glm::cross(right, direction));
	}
/// Depends on Camera::calculateUp to be called first
	inline glm::vec3 calculateRight() const {
		return glm::cross(direction, glm::vec3(0,1,0));
	}
	inline glm::mat4 getView() const {
		return glm::lookAt(pos, pos + direction, glm::vec3(0.0, 1.0, 0.0));
	}
};