#include "Transformation.h"
#include "Utils.h"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

namespace PixelBit {

Transformation::Transformation(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
	m_position = position;
	m_scale = scale;
	m_rotation_q = glm::quat(rotation);

	m_model_matrix = glm::identity<glm::mat4>();
}

Transformation::Transformation(): Transformation(glm::vec3(0), glm::vec3(0), glm::vec3(1)) {}

Transformation& Transformation::update() {
	m_model_matrix = glm::translate(glm::mat4(1.0f), m_position);
	m_model_matrix = m_model_matrix * glm::toMat4(m_rotation_q);
	m_model_matrix = glm::scale(m_model_matrix, m_scale);

	return *this;
}

Transformation& Transformation::update_rotate_translate() {
	m_model_matrix = glm::toMat4(m_rotation_q);
	m_model_matrix = glm::translate(m_model_matrix, m_position);

	return *this;
}

Transformation& Transformation::update_rotate_neg_translate() {
	m_model_matrix = glm::toMat4(m_rotation_q);
	m_model_matrix = glm::translate(m_model_matrix, -m_position);

	return *this;
}

glm::mat4 Transformation::matrix() const { return m_model_matrix; }
glm::mat4 Transformation::matrix(bool update) {
	if (update) this->update();
	return m_model_matrix;
}
const glm::mat4* Transformation::matrix_a() const { return &m_model_matrix; }
glm::mat4* Transformation::matrix_a(bool update) {
	if (update) this->update();
	return &m_model_matrix;
}

glm::vec3 Transformation::position() const { return m_position; }
glm::quat Transformation::rotation() { return m_rotation_q; }
glm::vec3 Transformation::rotation_euler() const { return glm::eulerAngles(m_rotation_q); }
glm::vec3 Transformation::scale() const { return m_scale; }

Transformation& Transformation::translate(float x, float y, float z) {
	m_position += glm::vec3(x, y, z);
	return *this;
}

Transformation& Transformation::position(float x, float y, float z) {
	m_position = glm::vec3(x, y, z);
	return *this;
}

Transformation& Transformation::position(glm::vec3 pos) {
	m_position = pos;
	return *this;
}

Transformation& Transformation::add_euler(float x, float y, float z) {
	m_rotation_q = glm::quat(rotation_euler() + glm::vec3(x, y, z));
	return *this;
}

Transformation& Transformation::set_euler(float x, float y, float z) {
	m_rotation_q = glm::quat(glm::vec3(x, y, z));
	return *this;
}

Transformation& Transformation::rotation(glm::quat rot) {
	m_rotation_q = rot;
	return *this;
}

Transformation& Transformation::scale(float xyz) {
	m_scale = glm::vec3(xyz);
	return *this;
}

Transformation& Transformation::scale(float x, float y, float z) {
	m_scale = glm::vec3(x, y, z);
	return *this;
}

Transformation& Transformation::scale(glm::vec3 sca) {
	m_scale = sca;
	return *this;
}

Transformation& Transformation::center(Window& window) {
	m_position = glm::vec3(window.width() * window.device_pixel_ratio() / 2, window.height() * window.device_pixel_ratio() / 2, m_position.z);
	return *this;
}

Transformation& Transformation::forwards(glm::vec3 offset) {
	return forwards(offset.x, offset.y, offset.z);
}

Transformation& Transformation::forwards(float x, float y, float z) {
	full_forwards(glm::vec3(x, y, z), glm::vec3(1, 0, 1));
	m_position.y += y;
	return *this;
}

Transformation& Transformation::full_forwards(glm::vec3 offset) {
	m_position += glm::inverse(m_rotation_q) * offset;

	return *this;
}

Transformation& Transformation::full_forwards(glm::vec3 offset, glm::vec3 axis_multiplier) {
	glm::vec3 translation = glm::inverse(m_rotation_q) * offset;
	m_position.x += translation.x * axis_multiplier.x;
	m_position.y += translation.y * axis_multiplier.y;
	m_position.z += translation.z * axis_multiplier.z;

	return *this;
}

Transformation& Transformation::full_forwards(float x, float y, float z) {
	return full_forwards(glm::vec3(x, y, z));
}

Transformation& Transformation::relative_rotate(glm::vec3 axis, float angle) {
	glm::quat update = glm::angleAxis(angle, axis);
	m_rotation_q = update * m_rotation_q ;

	return *this;
}

Transformation& Transformation::relative_rotate_x(float angle) {
	return relative_rotate(glm::vec3(1.0f, 0, 0), angle);
}

Transformation& Transformation::relative_rotate_y(float angle) {
	return relative_rotate(glm::vec3(0, 1.0f, 0), angle);
}

Transformation& Transformation::relative_rotate_z(float angle) {
	return relative_rotate(glm::vec3(0, 0, 1.0f), angle);
}

Transformation& Transformation::world_rotate(glm::vec3 axis, float angle) {
	glm::quat update = glm::angleAxis(angle, axis);
	m_rotation_q = m_rotation_q * update;

	return *this;
}

Transformation& Transformation::world_rotate_x(float angle) {
	return world_rotate(glm::vec3(1.0f, 0, 0), angle);
}

Transformation& Transformation::world_rotate_y(float angle) {
	return world_rotate(glm::vec3(0, 1.0f, 0), angle);
}

Transformation& Transformation::world_rotate_z(float angle) {
	return world_rotate(glm::vec3(0, 0, 1.0f), angle);
}

glm::mat4 Transformation::model_view(glm::mat4 view, bool update) {
	return view * matrix(update);
}

}