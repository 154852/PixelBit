#pragma once

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

namespace PixelBit {
	class VertexBuffer {
	private:
		int m_buffer_ID;
		int m_write_mode;
		int m_type;
		int m_stride;
		bool m_active { true };

		static void set_bound(int type, int id);
	public:
		VertexBuffer(int id, int writeMode, int type, int stride);
		void data(GLfloat* vertices, int length, int cellLength, int location);
		void update(GLfloat* vertices, int length);
		void index_data(GLint* indices, int length);

		int stride() const;
		int handle() const;
		int write_mode() const;

		int enable(int location);
		int disable(int location);

		void bind();
		static void unbind();

		static int gen_id();

		static VertexBuffer& create_flat_shaded_normals(int location, float* vertices, int* indices, int indicesLength);
	};
}