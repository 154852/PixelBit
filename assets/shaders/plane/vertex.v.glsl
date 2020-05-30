#version 330

precision highp float;

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoordI;

uniform mat4 modelMatrix;

out vec2 texCoord;

void main() {
	texCoord = texCoordI;
	gl_Position = modelMatrix * vec4(position, 1);
}