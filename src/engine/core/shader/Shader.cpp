#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Utils.h"
#include <iostream>

namespace PixelBit {
void Shader::set_bound(int id) {
	static int bound = 0;

	if (id != bound) {
		bound = id;
		glUseProgram(id);
	}
}

Shader::Shader() {
	m_handle = glCreateProgram();
}

int Shader::handle() const { return m_handle; }

Shader& Shader::compile() {
	glLinkProgram(m_handle);

	int status; glGetProgramiv(m_handle, GL_COMPILE_STATUS, &status);
	int logLength; glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0){
		std::vector<char> errorMessage(logLength);
		glGetProgramInfoLog(m_handle, logLength, NULL, errorMessage.data());
		std::cout << std::string(errorMessage.begin(), errorMessage.end());
	}

	if (m_vertex_ID != 0) glDetachShader(m_handle, m_vertex_ID);
	if (m_fragment_ID != 0) glDetachShader(m_handle, m_fragment_ID);

	return *this;
}

void Shader::bind() { Shader::set_bound(m_handle); }
void Shader::unbind() { Shader::set_bound(0); }

void Shader::create_uniform(const char* name) {
	int location = glGetUniformLocation(m_handle, name);
	if (location < 0) std::cout << "Warning: Uniform '" << name << "' not used in GLSL" << std::endl;
	else m_uniforms[std::string(name)] = location;
}

void Shader::set_uniform(std::string name, float* data, int length) {
	switch (length) {
		case 2: return glUniform2fv(m_uniforms[name], 1, data);
		case 3: return glUniform3fv(m_uniforms[name], 1, data);
		case 4: return glUniform4fv(m_uniforms[name], 1, data);
		case 16: return glUniformMatrix4fv(m_uniforms[name], 1, false, data);
		default: throw std::runtime_error(concat("si", "Could not find uniform of length ", length));
	}
}

void Shader::set_uniform(std::string name, glm::mat4 data) {
	glUniformMatrix4fv(m_uniforms[name], 1, false, glm::value_ptr(data));
}

void Shader::set_uniform(std::string name, int data) {
	glUniform1i(m_uniforms[name], data);
}

void Shader::set_uniform(std::string name, float data) {
	glUniform1f(m_uniforms[name], data);
}

void Shader::set_uniform(std::string name, glm::vec2 data) {
	glUniform2fv(m_uniforms[name], 1, glm::value_ptr(data));
}

void Shader::set_uniform(std::string name, glm::vec3 data) {
	glUniform3fv(m_uniforms[name], 1, glm::value_ptr(data));
}

void Shader::set_uniform(std::string name, glm::vec4 data) {
	glUniform4fv(m_uniforms[name], 1, glm::value_ptr(data));
}

Shader& Shader::uniform(std::string name, float* data, int length) {
	bind();
	if (m_uniforms.find(name) == m_uniforms.end()) create_uniform(name.c_str());
	set_uniform(name, data, length);

	return *this;
}

Shader& Shader::uniform(std::string name, glm::mat4 data) {
	bind();
	if (m_uniforms.find(name) == m_uniforms.end()) create_uniform(name.c_str());
	set_uniform(name, data);

	return *this;
}

Shader& Shader::uniform(std::string name, int data) {
	bind();
	if (m_uniforms.find(name) == m_uniforms.end()) create_uniform(name.c_str());
	set_uniform(name, data);

	return *this;
}

Shader& Shader::uniform(std::string name, float data) {
	bind();
	if (m_uniforms.find(name) == m_uniforms.end()) create_uniform(name.c_str());
	set_uniform(name, data);

	return *this;
}

Shader& Shader::uniform(std::string name, glm::vec2 data) {
	bind();
	if (m_uniforms.find(name) == m_uniforms.end()) create_uniform(name.c_str());
	set_uniform(name, data);

	return *this;
}

Shader& Shader::uniform(std::string name, glm::vec3 data) {
	bind();
	if (m_uniforms.find(name) == m_uniforms.end()) create_uniform(name.c_str());
	set_uniform(name, data);

	return *this;
}

Shader& Shader::uniform(std::string name, glm::vec4 data) {
	bind();
	if (m_uniforms.find(name) == m_uniforms.end()) create_uniform(name.c_str());
	set_uniform(name, data);

	return *this;
}

void Shader::render(Mesh& mesh) {
	bind();
	
#ifdef SHADER_VALIDATE
	glValidateProgram(m_handle);
	
	int status; glGetProgramiv(m_handle, GL_VALIDATE_STATUS, &status);
	if (status != GL_TRUE){
		int logLength; glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> errorMessage(logLength);
		glGetProgramInfoLog(m_handle, logLength, NULL, errorMessage.data());
		throw std::runtime_error(std::string(errorMessage.begin(), errorMessage.end()));
	}
#endif

	mesh.render();
	Shader::unbind();
}

int Shader::create_shader(int programID, std::string shaderCode, int type) {
	int shaderID = glCreateShader(type);
	if (shaderID == 0) throw std::runtime_error("Error creating shader");

	int len = shaderCode.length();
	const char* str = shaderCode.c_str();
	glShaderSource(shaderID, 1, &str, &len);
	glCompileShader(shaderID);

	int status; glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	int logLength; glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0){
		std::vector<char> errorMessage(logLength);
		glGetShaderInfoLog(shaderID, logLength, NULL, errorMessage.data());
		throw std::runtime_error(std::string(errorMessage.begin(), errorMessage.end()));
	}

	glAttachShader(programID, shaderID);
	return shaderID;
}

Shader& Shader::create(std::string vertex, std::string fragment, bool compile) {
	Shader* shader = new Shader();
	shader->m_vertex_ID = Shader::create_shader(shader->handle(), vertex, GL_VERTEX_SHADER);
	shader->m_fragment_ID = Shader::create_shader(shader->handle(), fragment, GL_FRAGMENT_SHADER);
	if (compile) shader->compile();

	return *shader;
}

Shader& Shader::create(const char* name, bool compile) {
	return create(
		read_file(concat("sss", "assets/shaders/", name, "/vertex.v.glsl").c_str()),
		read_file(concat("sss", "assets/shaders/", name, "/fragment.f.glsl").c_str())
	);
}

Shader& Shader::texture(Texture& tex, int location) {
	if (location < 0 || location > 31) throw std::runtime_error("Texture locations can only be in the range [0, 31]");

	glActiveTexture(GL_TEXTURE0 + location);
	tex.bind();

	return *this;
}

}