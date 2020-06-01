#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform int texture_transform_mode;

vec2 texture_transform(float x, float y) {
	switch (texture_transform_mode) {
		case 0: return vec2(x, y);
		case 1: return vec2(1 - x, y);
		case 2: return vec2(x, 1 - y);
		case 3: return vec2(1 - x, 1 - y);
		default: return vec2(x, y);
	}
}

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = texture_transform(aTexCoords.x, aTexCoords.y);
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}