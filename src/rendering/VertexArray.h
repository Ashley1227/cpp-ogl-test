#pragma once

class VertexArray {
public:
	/// size in bytes
	unsigned int size;
	/// length of the array
	unsigned int length;
	float* data;

	VertexArray();
	VertexArray(unsigned int size, unsigned int length, float* data);

	operator float*() const {
		return data;
	}
};