#include "IndexArray.h"
#include "../global.h"

IndexArray::IndexArray() {}
IndexArray::IndexArray(unsigned int size, unsigned int length, unsigned int* data): size(size), length(length), raw(data) {}