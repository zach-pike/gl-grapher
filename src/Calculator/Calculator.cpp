#include "Calculator.hpp"

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include "Utility/GL/VertexArray/VertexArray.hpp"
#include "Utility/GL/Buffer/Buffer.hpp"
#include "Utility/GL/Shader/Shader.hpp"

#include <vector>
#include <thread>
#include <chrono>

Calculator::Calculator():
    logger("Calculator", Logger::FGColors::GREEN) {}

Calculator::~Calculator() {}

void Calculator::runUI() {
    if (!glfwInit()) {
        logger.error("Failed to initialize GLFW!");
        exit(1);        
    }

    GLFWwindow* window = glfwCreateWindow(1024, 768, "Graph-iCal", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        logger.error("Failed to initialize GLAD!");
        glfwTerminate();
        exit(1);
    }

    // OpenGL ready!
    VertexArray vao;
    vao.initVAO();
    vao.useVAO();

    Shader lineShader("shader/lineShader/vertex.glsl", "shader/lineShader/fragment.glsl");
    Buffer<glm::vec2> lineBuffer;

    lineShader.loadShaders();

    lineBuffer.init();

    // Put some data in the buffer
    std::vector<glm::vec2> ptsData = {
        glm::vec2(-1, -1),
        glm::vec2( -.7, -.4),
        glm::vec2(0, 0),
        glm::vec2(.5, -.5),
        glm::vec2(1, 1)
    };

    lineBuffer.bufferData(ptsData);

    glClearColor(.25f, .5f, .75f, 1.0f);

    while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
        double frameStartTime = glfwGetTime();

        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, lineBuffer.getBufferID());
        glVertexAttribPointer(
            0,                  // attribute 0.
            2,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

        glDrawArrays(GL_LINE_STRIP, 0, lineBuffer.size());

        glDisableVertexAttribArray(0);

        double frameEndTime = glfwGetTime();
        double frameTimeMS = (frameEndTime - frameStartTime) * 1e6;
        const double requiredFrameTimeMS = 16667;
        double sleepTime = requiredFrameTimeMS - frameTimeMS;

        if (sleepTime > 0) {
            std::this_thread::sleep_for(std::chrono::microseconds((std::int64_t)sleepTime));
        }

        glfwSwapBuffers(window);
    }

    // Unload everything
    lineBuffer.uninit();
    lineShader.unloadShader();
    vao.uninitVAO();

    glfwTerminate();
}
