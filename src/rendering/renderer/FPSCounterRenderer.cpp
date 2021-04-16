#include "FPSCounterRenderer.h"

#include "../../global.h"

void FPSCounterRenderer::countFrame(double start, double end) {
	c_ms += end-start;
	c_fps++;
	total++;

	if(glfwGetTime() > nextTimestamp) {
		fps = c_fps;
		ms = c_ms / c_fps;

		c_fps = 0;
		c_ms = 0;

		nextTimestamp = glfwGetTime() + 1.0;
	}
}