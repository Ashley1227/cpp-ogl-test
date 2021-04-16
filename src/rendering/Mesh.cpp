#include "Mesh.h"
#include "../global.h"

Mesh::Mesh() {}
Mesh::Mesh(VertexArray vertexArray, IndexArray indexArray): vertexArray(vertexArray), indexArray(indexArray) {}

void Mesh::genBuffers() {
    genVAO();
    genVBO();
    genEBO();

    genVertexAttribs();
}
void Mesh::genVertexAttribs() {
    bind();
    // coords
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    // Tex coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}
void Mesh::genVAO() {
    glGenVertexArrays(1, &vao);
}
void Mesh::genVBO() {
    glGenBuffers(1, &vbo);
}
void Mesh::genEBO() {
    glGenBuffers(1, &ebo);
}

void Mesh::deleteBuffers() {
    deleteVAO();
    deleteVBO();
    deleteEBO();
}
void Mesh::deleteVAO() {
    glDeleteVertexArrays(1, &vao);
}
void Mesh::deleteVBO() {
    glDeleteBuffers(1, &vbo);
}
void Mesh::deleteEBO() {
    glDeleteBuffers(1, &ebo);
}

void Mesh::bind() {
    bindVAO();
    bindVBO();
    bindEBO();
}
void Mesh::bindVAO() {
    glBindVertexArray(vao);
}
void Mesh::bindVBO() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}
void Mesh::bindEBO() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void Mesh::unbind() {
    unbindVAO();
    unbindVBO();
    unbindEBO();
}
void Mesh::unbindVAO() {
    glBindVertexArray(0);
}
void Mesh::unbindVBO() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Mesh::unbindEBO() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
