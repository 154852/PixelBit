#version 330

precision highp float;

uniform sampler2D image;

out vec4 color;

in vec2 texCoord;

void main() {
	color = vec4(1 - texture(image, texCoord).rgb, 1);
	// color = vec4(1, 0, 0, 1);
}