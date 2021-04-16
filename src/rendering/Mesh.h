#pragma once

#include "VertexArray.h"
#include "IndexArray.h"

struct Mesh {
    unsigned int vao, vbo, ebo;

	VertexArray vertexArray;
	IndexArray indexArray;

	Mesh();
	Mesh(VertexArray vertexArray, IndexArray indexArray);

	void genBuffers();
	void genVertexAttribs();
	void genVAO();
	void genVBO();
	void genEBO();

	void deleteBuffers();
	void deleteVAO();
	void deleteVBO();
	void deleteEBO();

	void bind();
	void bindVAO();
	void bindVBO();
	void bindEBO();

	void unbind();
	void unbindVAO();
	void unbindVBO();
	void unbindEBO();
};