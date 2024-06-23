#include "VertexArray.hpp"

VertexArray::VertexArray() {}
VertexArray::~VertexArray() {
    if (initialized) uninitVAO();
}

void VertexArray::initVAO() {
    if (initialized) return;

    glGenVertexArrays(1, &vao);
    initialized = true;
}

void VertexArray::uninitVAO() {
    if (!initialized) return;

    glDeleteVertexArrays(1, &vao);
    initialized = false;
}

void VertexArray::useVAO() {
    glBindVertexArray(vao);
}