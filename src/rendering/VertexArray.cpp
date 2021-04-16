#include "VertexArray.h"
#include "../global.h"

VertexArray::VertexArray(): size(0), data(0) {}
VertexArray::VertexArray(unsigned int size, unsigned int length, float* data): size(size), length(length), data(data) {}