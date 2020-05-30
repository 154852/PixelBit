#pragma once

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <map>
#include <iostream>
#include <glm/glm.hpp>
#include "../mesh/Mesh.h"
#include "../texture/Texture.h"

namespace PixelBit {
	class Shader {
	private:
		int m_handle;
		int m_vertex_ID { 0 };
		int m_fragment_ID { 0 };
		std::map<std::string, int> m_uniforms;
		bool m_active { false };

		static void set_bound(int id);

		static int create_shader(int programID, std::string shaderCode, int type);

		void create_uniform(const char* name);
		void set_uniform(std::string name, float* data, int length);
		void set_uniform(std::string name, glm::mat4 data);
		void set_uniform(std::string name, int data);
		void set_uniform(std::string name, float data);
		void set_uniform(std::string name, glm::vec2 data);
		void set_uniform(std::string name, glm::vec3 data);
		void set_uniform(std::string name, glm::vec4 data);
	public:
		Shader();
		int handle() const;
		Shader& compile();

		Shader& uniform(std::string name, float* data, int length);
		Shader& uniform(std::string name, glm::mat4 data);
		Shader& uniform(std::string name, int data);
		Shader& uniform(std::string name, float data);
		Shader& uniform(std::string name, glm::vec2 data);
		Shader& uniform(std::string name, glm::vec3 data);
		Shader& uniform(std::string name, glm::vec4 data);
		template<class T>
		Shader& uniform(std::string name, T data) {
			return uniform(name, (T) data);
		}

		Shader& texture(Texture& tex, int location);

		void render(Mesh& mesh);

		void bind();
		static void unbind();

		static Shader& create(std::string vertex, std::string fragment, bool compile = true);
		static Shader& create(const char* name, bool compile = true);
	};
}