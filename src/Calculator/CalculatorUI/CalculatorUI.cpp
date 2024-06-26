#include "CalculatorUI.hpp"

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/misc/cpp/imgui_stdlib.h"

#include "Utility/GL/VertexArray/VertexArray.hpp"
#include "Utility/GL/Buffer/Buffer.hpp"
#include "Utility/GL/Shader/Shader.hpp"

#include <vector>
#include <thread>
#include <chrono>

CalculatorUI::CalculatorUI(CalculatorWorker& _worker):
    logger("Calculator", Logger::FGColors::GREEN),
    viewportInfo(CalculatorWorker::CalculatorViewportPosition{ -10, -10, 20, 20 }),
    worker(_worker)
{
    worker.setViewportInfo(viewportInfo);
}

CalculatorUI::~CalculatorUI() {}

void CalculatorUI::runUI() {
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

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    // OpenGL ready!
    VertexArray vao;
    vao.initVAO();
    vao.useVAO();

    // Create shader and buffer
    Shader lineShader("shader/lineShader/vertex.glsl", "shader/lineShader/fragment.glsl");
    Buffer<glm::vec2> lineBuffer;

    // Load shader and init line buffer
    lineShader.loadShaders();
    lineBuffer.init();

    // Set clear color
    glClearColor(.25f, .5f, .75f, 1.0f);

    std::string expression;


    while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
        double frameStartTime = glfwGetTime();

        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        // ImGui stuff
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Load new line data if available
        if (worker.isNewDataAvailable()) {
            auto data = worker.getData();
            lineBuffer.bufferData(data);
        }

        ImGui::Begin("Controls");

            ImGui::InputText("Expression", &expression);
            if (ImGui::Button("Start")) {
                worker.setExpression(expression);
            }

            if (ImGui::Button("Up")) {
                viewportInfo.y += 1;
                worker.setViewportInfo(viewportInfo);
            }
            if (ImGui::Button("Down")) {
                viewportInfo.y -= 1;
                worker.setViewportInfo(viewportInfo);
            }
            if (ImGui::Button("Left")) {
                viewportInfo.x -= 1;
                worker.setViewportInfo(viewportInfo);
            }
            if (ImGui::Button("Right")) {
                viewportInfo.x += 1;
                worker.setViewportInfo(viewportInfo);
            }

        ImGui::End();

        lineShader.useShader();

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

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Frame limiter
        double frameEndTime = glfwGetTime();
        double frameTimeMS = (frameEndTime - frameStartTime) * 1e6;
        const double requiredFrameTimeMS = 16667;
        double sleepTime = requiredFrameTimeMS - frameTimeMS;

        if (sleepTime > 0) {
            std::this_thread::sleep_for(std::chrono::microseconds((std::int64_t)sleepTime));
        }

        // Make viewport always the same size as window
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        glfwSwapBuffers(window);
    }

    // Unload everything
    lineBuffer.uninit();
    lineShader.unloadShader();
    vao.uninitVAO();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}
