#version 330

precision highp float;

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoordI;

out vec2 texCoord;

void main() {
	texCoord = texCoordI;
	gl_Position = vec4(position, 1);
}