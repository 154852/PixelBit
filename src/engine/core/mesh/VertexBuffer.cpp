#include "VertexBuffer.h"
#include <iostream>
#include <glm/glm.hpp>

namespace PixelBit {
VertexBuffer::VertexBuffer(int id, int writeMode, int type, int stride) {
	m_buffer_ID = id;
	m_write_mode = writeMode;
	m_type = type;
	m_stride = stride;
}

void VertexBuffer::data(GLfloat* vertices, int length, int cellLength, int location) {
	bind();

	glBufferData(m_type, sizeof(GLfloat) * length, vertices, m_write_mode);
	glVertexAttribPointer(location, cellLength, GL_FLOAT, false, 0, 0);
	enable(location);
}

void VertexBuffer::update(GLfloat* vertices, int length) {
	bind();
	glBufferData(m_type, sizeof(GLfloat) * length, vertices, m_write_mode);
}

void VertexBuffer::index_data(GLint* data, int length) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffer_ID);
	glBufferData(m_type, sizeof(GLint) * length, data, m_write_mode);
}

int VertexBuffer::stride() const { return m_stride; }
int VertexBuffer::handle() const { return m_buffer_ID; }
int VertexBuffer::write_mode() const { return m_write_mode; }

int VertexBuffer::enable(int location) {
	for (int i = 0; i < m_stride; i++) glEnableVertexAttribArray(location + i);
	return location + m_stride;
}

int VertexBuffer::disable(int location) {
	for (int i = 0; i < m_stride; i++) glDisableVertexAttribArray(location + i);
	return location + m_stride;
}

void VertexBuffer::bind() { VertexBuffer::set_bound(m_type, m_buffer_ID); }
void VertexBuffer::unbind() { VertexBuffer::set_bound(GL_ARRAY_BUFFER, 0); }

void VertexBuffer::set_bound(int type, int id) {
	static int bound = 0;
	if (id != bound) {
		bound = id;
		glBindBuffer(type, id);
	}
}

int VertexBuffer::gen_id() {
	int id;
	glGenBuffers(1, (GLuint*) &id);
	return id;
}

VertexBuffer& VertexBuffer::create_flat_shaded_normals(int location, float* vertices, int* indices, int indicesLength) {
	float* normals = (float*) calloc(indicesLength * 3, sizeof(float));

	for (int i = 0; i < indicesLength; i += 3) {
		glm::vec3 verts[3] = {
			glm::vec3(vertices[indices[i] * 3], vertices[(indices[i] * 3) + 1], vertices[(indices[i] * 3) + 2]),
			glm::vec3(vertices[indices[i + 1] * 3], vertices[(indices[i + 1] * 3) + 1], vertices[(indices[i + 1] * 3) + 2]),
			glm::vec3(vertices[indices[i + 2] * 3], vertices[(indices[i + 2] * 3) + 1], vertices[(indices[i + 2] * 3) + 2])
		};

		glm::vec3 e1 = verts[1] - verts[0];
		glm::vec3 e2 = verts[2] - verts[0];
		glm::vec3 normal = glm::cross(e1, e2);

		normals[ indices[i    ] * 3     ] = normal.x;
		normals[ indices[i + 1] * 3     ] = normal.x;
		normals[ indices[i + 2] * 3     ] = normal.x;
		normals[(indices[i    ] * 3) + 1] = normal.y;
		normals[(indices[i + 1] * 3) + 1] = normal.y;
		normals[(indices[i + 2] * 3) + 1] = normal.y;
		normals[(indices[i    ] * 3) + 2] = normal.z;
		normals[(indices[i + 1] * 3) + 2] = normal.z;
		normals[(indices[i + 2] * 3) + 2] = normal.z;
	}

	VertexBuffer* buffer = new VertexBuffer(VertexBuffer::gen_id(), GL_STATIC_DRAW, GL_ARRAY_BUFFER, 1);
	buffer->data(normals, indicesLength * 3, 3, location);

	return *buffer;
}

}