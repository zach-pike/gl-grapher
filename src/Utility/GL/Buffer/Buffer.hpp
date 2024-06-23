#pragma once

#include "glad/glad.h"

#include <span>


template <typename T>
class Buffer {
private:
    GLuint buffer;
    bool   ready = false;

    std::size_t lastBufferedCount = 0;
public:
    Buffer() {}
    ~Buffer() {
        if (ready) uninit();
    }

    void init() {
        if (ready) return;

        glGenBuffers(1, &buffer);
        ready = true;
    }

    void uninit() {
        if (!ready) return;

        glDeleteBuffers(1, &buffer);
        ready = false;
    }

    void bufferData(std::span<T> data) {
        if (!ready) init();

        glBindBuffer(GL_ARRAY_BUFFER, buffer);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(T)*data.size(), data.data(), GL_STATIC_DRAW);

        lastBufferedCount = data.size();
    }

    GLuint getBufferID() {
        return buffer;
    }

    std::size_t size() const {
        return lastBufferedCount;
    }
};