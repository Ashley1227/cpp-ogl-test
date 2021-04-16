#pragma once

// common includes, helps fix include loops
#include "rendering/GLHelper.h"

#include <cstdio>
#include <iostream>

#include "rendering/camera/Camera.h"
#include "world/World.h"

extern GLFWwindow* WINDOW;
extern Camera CAMERA;
extern World WORLD;

namespace settings {
	extern unsigned short renderDistance;

	extern float near;
	extern float far;

	extern float fov;
}