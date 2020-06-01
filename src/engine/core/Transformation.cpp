#include "Transformation.h"
#include "Utils.h"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

namespace PixelBit {

Transformation::Transformation(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
	m_position = position;
	m_rotation = rotation;
	m_scale = scale;

	m_model_matrix = glm::identity<glm::mat4>();
}

Transformation::Transformation(): Transformation(glm::vec3(0), glm::vec3(0), glm::vec3(1)) {}

Transformation& Transformation::update() {
	m_model_matrix = glm::translate(glm::mat4(1.0f), m_position);
	m_model_matrix = glm::rotate(m_model_matrix, m_rotation.x, glm::vec3(1, 0, 0));
	m_model_matrix = glm::rotate(m_model_matrix, m_rotation.y, glm::vec3(0, 1, 0));
	m_model_matrix = glm::rotate(m_model_matrix, m_rotation.z, glm::vec3(0, 0, 1));
	m_model_matrix = glm::scale(m_model_matrix, m_scale);

	return *this;
}

Transformation& Transformation::update_rotate_translate() {
	m_model_matrix = glm::rotate(glm::mat4(1.0f), m_rotation.x, glm::vec3(1, 0, 0));
	m_model_matrix = glm::rotate(m_model_matrix, m_rotation.y, glm::vec3(0, 1, 0));
	m_model_matrix = glm::rotate(m_model_matrix, m_rotation.z, glm::vec3(0, 0, 1));
	m_model_matrix = glm::translate(m_model_matrix, m_position);

	return *this;
}

Transformation& Transformation::update_rotate_neg_translate() {
	m_model_matrix = glm::rotate(glm::mat4(1.0f), m_rotation.x, glm::vec3(1, 0, 0));
	m_model_matrix = glm::rotate(m_model_matrix, m_rotation.y, glm::vec3(0, 1, 0));
	m_model_matrix = glm::rotate(m_model_matrix, m_rotation.z, glm::vec3(0, 0, 1));
	m_model_matrix = glm::translate(m_model_matrix, -m_position);

	return *this;
}

glm::mat4 Transformation::matrix() const { return m_model_matrix; }
glm::mat4 Transformation::matrix(bool update) {
	if (update) this->update();
	return m_model_matrix;
}
glm::vec3 Transformation::position() const { return m_position; }
glm::vec3 Transformation::rotation() const { return m_rotation; }
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

Transformation& Transformation::rotate(float x, float y, float z) {
	m_rotation += glm::vec3(x, y, z);
	return *this;
}

Transformation& Transformation::rotation(float x, float y, float z) {
	m_rotation = glm::vec3(x, y, z);
	return *this;
}

Transformation& Transformation::rotation(glm::vec3 rot) {
	m_rotation = rot;
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
	if (z != 0) {
		m_position.x += sin(m_rotation.y) * -z;
		m_position.z += cos(m_rotation.y) * z;
	}

	if (x != 0) {
		m_position.x += sin(m_rotation.y - M_PI_2) * -x;
		m_position.z += cos(m_rotation.y - M_PI_2) * x;
	}

	m_position.y += y;

	return *this;
}

glm::mat4 Transformation::model_view(glm::mat4 view, bool update) {
	return view * matrix(update);
}

}