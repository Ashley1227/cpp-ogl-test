#include <cstdio>
#include "Camera.h"

Camera::Camera():
pos(0,0,4),
yaw(0), pitch(0),
direction(0,0,-1),
sensitivity(0.4f),
showCursor(false)
{}
void Camera::render() {
	right = calculateRight();
	up = calculateUp();
}
void Camera::processInput(GLFWwindow* window, double dt) {
	float cameraSpeed = 5.0f * dt; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraSpeed *= 2;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		pos += direction * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		pos -= direction * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		pos -= right * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		pos += right * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		pos += up * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		pos -= up * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		showCursor = !showCursor;
	if(showCursor)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	double xOffset = mouseX - lastX;
	double yOffset = mouseY - lastY;

	yaw += xOffset * sensitivity;
	pitch += -yOffset * sensitivity;

	if(pitch > 89.0f)
		pitch = 89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;

	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction = glm::normalize(direction);

	lastX = mouseX;
	lastY = mouseY;

//	glfwSetCursorPos(window, 0, 0);
}