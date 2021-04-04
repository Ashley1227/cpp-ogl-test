#pragma once

#include <vector>

class VertexArray {
public:
	/// size in bytes
	unsigned int size;
	float* raw;

	VertexArray(): VertexArray(0, 0) {}
	VertexArray(unsigned int size, float* data): size(size), raw(data) {}

	~VertexArray() {
		delete[] raw;
	}

	operator float*() const {
		return raw;
	}
};
