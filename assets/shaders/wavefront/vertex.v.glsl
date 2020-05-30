#version 330

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normalI;
layout (location = 2) in vec2 texCoordI;

out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;

vec3 fragmentPosition(mat4 modelMatrix, vec3 vertexPosition) {
    return vec3(modelMatrix * vec4(vertexPosition, 1));
}

vec3 nonUniformNormal(mat4 modelMatrix, vec3 normal) {
    return mat3(transpose(inverse(modelMatrix))) * normal;
}

void main() {
	gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);

    texCoord = texCoordI;
    normal = nonUniformNormal(mat4(1.0), normalI);
    fragPos = fragmentPosition(mat4(1.0), position);
}