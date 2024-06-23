#version 330 core

// Ouput data
out vec3 color;

in vec3 finalColor;

void main() {
	color = finalColor;
}