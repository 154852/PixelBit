#version 330

struct AmbientLight {
    vec3 color;
    float intensity;
};

vec3 ambient(AmbientLight light) {
    return light.color * light.intensity;
}

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float specularStrength;
};

vec3 diffuse(PointLight light, vec3 normal, vec3 fragPos) {
    vec3 lightDirection = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDirection), 0);
    return diff * light.intensity * light.color;
}

vec3 flatShade(vec3 fragPos) {
    return normalize(cross(dFdx(fragPos), dFdy(fragPos)));
}

vec3 specular(PointLight light, vec3 normal, vec3 fragPos, vec3 cameraPos) {
    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    return light.specularStrength * light.intensity * spec * light.color;
}

vec3 phong(PointLight light, vec3 normal, vec3 fragPos, vec3 cameraPos) {
    return diffuse(light, normal, fragPos) + specular(light, normal, fragPos, cameraPos);
}

vec3 sceneLighting(AmbientLight ambientLight, PointLight light, vec3 normal, vec3 fragPos, vec3 cameraPos) {
    return ambient(ambientLight) + phong(light, normal, fragPos, cameraPos);
}

vec3 sceneColor(AmbientLight ambientLight, PointLight light, vec3 normal, vec3 fragPos, vec3 cameraPos, vec3 color) {
    return sceneLighting(ambientLight, light, normal, fragPos, cameraPos) * color;
}

struct Scene {
	AmbientLight ambientLight;
	PointLight pointLight;
};

struct Material {
	vec3 ambient;
	vec3 diffuse;
};

uniform Material material;
uniform Scene scene;
uniform vec3 cameraPosition;
uniform int forceFlatShade;

uniform sampler2D diffuse_map;
uniform int textured;

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

void main() {
	vec3 finalNormal = forceFlatShade == 1? flatShade(fragPos):normal;
    vec3 diffuse_color = textured == 1? texture(diffuse_map, vec2(texCoord.x, 1.0 - texCoord.y)).rgb * material.diffuse:material.diffuse;

	fragColor = vec4(sceneColor(scene.ambientLight, scene.pointLight, finalNormal, fragPos, cameraPosition, diffuse_color), 1);
    // fragColor = vec4(diffuse_color, 1);
    // fragColor = vec4(texCoord.x, texCoord.y, 0, 1);
}