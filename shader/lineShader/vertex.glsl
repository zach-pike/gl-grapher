#version 410 core

layout(location = 0) in vec2 vertexPos;

void main(){
    gl_Position.xyz = vec3(vertexPos, 0);
    gl_Position.w = 1.0;
}
