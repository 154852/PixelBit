#version 330

precision highp float;

uniform float c;
uniform sampler2D image;
uniform mat4 modelMatrix;

out vec4 color;

in vec2 texCoord;

void main() {
	// color = texture(image, texCoord) * c;
	color = vec4(vec3(modelMatrix[0][0]), 1);
}