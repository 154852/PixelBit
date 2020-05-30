#pragma once

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <vector>

#include "VertexBuffer.h"

namespace PixelBit {
	class Mesh {
	private:
		static void set_bound(int id);
		int m_vao_ID;
		std::vector<VertexBuffer*> m_buffers;
		int m_vertex_count;
		int m_idx { 0 };
		bool m_active { false };
	public:
		Mesh();

		VertexBuffer& indices(int data[], int length);
		VertexBuffer& vertex_buffer(GLfloat data[], int length, int cellLength);
		void render();

		void bind();
		static void unbind();

		static Mesh& plane();
	};
}