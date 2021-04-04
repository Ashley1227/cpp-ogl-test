#pragma once

#include "VertexArray.h"

class IndexArray {
public:
	/// size in bytes
	unsigned int size;
	unsigned int* raw;

	IndexArray(): IndexArray(0, 0) {}
	IndexArray(unsigned int size, unsigned int* data): size(size), raw(data) {}

	~IndexArray() {
		delete[] raw;
	}

	operator unsigned int*() const {
		return raw;
	}
};