#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace PixelBit {

PerspectiveCamera::PerspectiveCamera(Window& window) {
	update_projection(window); update_view();
}

PerspectiveCamera& PerspectiveCamera::update_projection(Window& window) {
	m_projection_matrix = glm::perspective(m_fov, (float) window.width() / (float) window.height(), m_znear, m_zfar);
	return *this;
}

void PerspectiveCamera::update_view() {
	m_transformation.update_rotate_neg_translate();
}

void PerspectiveCamera::set_near(float z_near) { m_znear = z_near; }
void PerspectiveCamera::set_far(float z_far) { m_zfar = z_far; }
void PerspectiveCamera::set_fov(float fov) { m_fov = fov; }
float PerspectiveCamera::znear() const { return m_znear; }
float PerspectiveCamera::zfar() const { return m_zfar; }
float PerspectiveCamera::fov() const { return m_fov; }

glm::mat4 PerspectiveCamera::projection() {
	return m_projection_matrix;
}

glm::mat4 PerspectiveCamera::projection(Window& window) {
	update_projection(window);
	return m_projection_matrix;
}

Transformation& PerspectiveCamera::transform() {
	return m_transformation;
}

glm::mat4 PerspectiveCamera::view() {
	return m_transformation.matrix();
}

glm::mat4 PerspectiveCamera::view(bool update) {
	if (update) update_view();
	return m_transformation.matrix();
}
};