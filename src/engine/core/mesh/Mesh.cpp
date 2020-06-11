#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "VertexBuffer.h"

#include "Mesh.h"

namespace PixelBit {
void Mesh::set_bound(int id) {
	static int bound = 0;
	if (id != bound) {
		bound = id;
		glBindVertexArray(id);
	}
}

Mesh::Mesh() {
	GLuint id;
	glGenVertexArrays(1, &id);
	m_vao_ID = id;
}

VertexBuffer& Mesh::indices(int data[], int length) {
	m_vertex_count = length;

	bind();
	VertexBuffer* buffer = new VertexBuffer(VertexBuffer::gen_id(), GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER, 1);
	m_buffers.push_back(buffer);
	buffer->index_data(data, length);
	Mesh::unbind();

	return *buffer;
}

VertexBuffer& Mesh::vertex_buffer(float data[], int length, int cellLength) {
	if (m_buffers.empty()) throw std::runtime_error("Indices must be the first buffer added");
	
	bind();
	VertexBuffer* buffer = new VertexBuffer(VertexBuffer::gen_id(), GL_STATIC_DRAW, GL_ARRAY_BUFFER, 1);
	m_buffers.push_back(buffer);
	buffer->data(data, length, cellLength, m_idx++);
	Mesh::unbind();

	return *buffer;
}

VertexBuffer& Mesh::vertex_buffer(VertexBuffer& buffer) {
	if (m_buffers.empty()) throw std::runtime_error("Indices must be the first buffer added");
	m_buffers.push_back(&buffer);
	return buffer;
}

void Mesh::render(glm::mat4* parent) {
	bind();
	glDrawElements(GL_TRIANGLES, m_vertex_count, GL_UNSIGNED_INT, 0);
}

void Mesh::bind() { Mesh::set_bound(m_vao_ID); }
void Mesh::unbind() { Mesh::set_bound(0); }

Mesh& Mesh::plane() {
	Mesh* mesh = new Mesh();

	mesh->indices((int[6]) {
		0, 1, 2,
		0, 2, 3
	}, 6);

	mesh->vertex_buffer((float[12]) {
		-1, -1, 0,
		 1, -1, 0,
		 1,  1, 0,
		-1,  1, 0
	}, 3 * 4, 3);

	mesh->vertex_buffer((float[12]) {
		0, 0, 1,
		0, 0, 1,
		0, 0, 1,
		0, 0, 1
	}, 3 * 4, 3);

	mesh->vertex_buffer((float[8]) {
		0, 1,
		1, 1,
		1, 0,
		0, 0
	}, 2 * 4, 2);

	return *mesh;
}

}