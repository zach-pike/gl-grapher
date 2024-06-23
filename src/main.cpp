#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "Utility/GL/Shader/Shader.hpp"
#include "Utility/GL/Buffer/Buffer.hpp"
#include "Utility/GL/VertexArray/VertexArray.hpp"

#include <iostream>
#include <vector>

int main() {
    if (!glfwInit()) {
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(1024, 768, "LearnOpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Couldn't load glad!\n";

        glfwTerminate();

        return 1;
    }

    VertexArray vao;
    vao.initVAO();
    vao.useVAO();

    Shader shader("shader/vertex.glsl", "shader/fragment.glsl");
    shader.loadShaders();

    Buffer<glm::vec2> vertexBuffer;
    Buffer<glm::vec3> colorBuffer;

    vertexBuffer.init();
    colorBuffer.init();

    glClearColor(.25f, .5f, .75f, 1.0f);

    std::vector<glm::vec2> vertexes = {
        glm::vec2(-1, -1),
        glm::vec2(0, 1),
        glm::vec2(1, -1)
    };

    std::vector<glm::vec3> colors = {
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 0, 1)
    };

    vertexBuffer.bufferData(vertexes);
    colorBuffer.bufferData(colors);

    shader.useShader();

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.getBufferID());
        glVertexAttribPointer(
            0,                  // attribute 0.
            2,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer.getBufferID());
        glVertexAttribPointer(
            1,                  // attribute 1.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

        glDrawArrays(GL_TRIANGLES, 0, vertexes.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glfwSwapBuffers(window);
    }

    vertexBuffer.uninit();
    colorBuffer.uninit();

    shader.unloadShader();
    vao.uninitVAO();

    glfwTerminate();
    return 0;
}