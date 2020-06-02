#pragma once

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <vector>

#include "../GL.h"
#include "VertexBuffer.h"

namespace PixelBit {
	class Mesh: public Renderable {
	private:
		static void set_bound(int id);
		int m_vao_ID;
		bool m_active { false };
	protected:
		std::vector<VertexBuffer*> m_buffers;
		int m_vertex_count;
		int m_idx { 0 };
	public:
		Mesh();

		VertexBuffer& indices(int data[], int length);
		VertexBuffer& vertex_buffer(GLfloat data[], int length, int cellLength);
		virtual void render(glm::mat4* parent) override;

		void bind();
		static void unbind();

		static Mesh& plane();
	};
}