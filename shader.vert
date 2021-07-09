#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 atexCoords;

out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() { 
	texCoords = atexCoords;
	gl_Position =  proj * view * model * vec4(position, 1.0);
}