#version 330 core

layout(location = 0) in vec2 vertexPos;
layout(location = 1) in vec3 color;

out vec3 finalColor;

void main(){
    gl_Position.xyz = vec3(vertexPos, 0);
    gl_Position.w = 1.0;

    finalColor = color;
}
