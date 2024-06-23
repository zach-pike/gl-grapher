#pragma once

#include "glad/glad.h"

class VertexArray {
private:
    bool initialized = false;
    GLuint vao;
public:
    VertexArray();
    ~VertexArray();

    void initVAO();
    void uninitVAO();

    void useVAO();
};
