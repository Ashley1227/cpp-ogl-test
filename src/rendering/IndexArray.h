#pragma once

class IndexArray {
public:
	/// size in bytes
	unsigned int size;
    /// length of the array
    unsigned int length;
	unsigned int* raw;

	IndexArray();
	IndexArray(unsigned int size, unsigned int length, unsigned int* data);

	operator unsigned int*() const {
		return raw;
	}
};