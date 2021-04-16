#include "global.h"

GLFWwindow* WINDOW;
Camera CAMERA;
World WORLD(&CAMERA);

namespace settings {
	unsigned short renderDistance = 512;

	float fov = 100.0f;

	float near = 0.1f;
	float far = 512.0f;
}