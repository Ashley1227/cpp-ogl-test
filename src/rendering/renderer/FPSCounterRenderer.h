#pragma once

class FPSCounterRenderer {
protected:
	double nextTimestamp;

	unsigned int c_fps;
	double c_ms;
public:
	unsigned long total;

	unsigned int fps;
	double ms;

	void countFrame(double start, double end);
};

