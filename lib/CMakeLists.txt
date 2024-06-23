cmake_minimum_required(VERSION 3.22.1)

add_subdirectory(glad/)

file(GLOB IMGUI_SRC imgui/*.cpp)
set(IMGUI_IMPL_SRC imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp)
set(IMGUI_STDLIB_SRC imgui/misc/cpp/imgui_stdlib.cpp)
add_library(ImGui ${IMGUI_SRC} ${IMGUI_IMPL_SRC} ${IMGUI_STDLIB_SRC})
target_include_directories(ImGui AFTER PUBLIC imgui/)