#include "PixelCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace PixelBit {

PixelCamera::PixelCamera(Window& window) {
	update_projection(window); update_view();
}

PixelCamera& PixelCamera::update_projection(Window& window) {
	m_projection_matrix = glm::ortho((float) 0, (float) window.width(), (float) window.height(), (float) 0, m_znear, m_zfar);
	return *this;
}

void PixelCamera::update_view() {
	m_transformation.update_rotate_neg_translate();
}

void PixelCamera::set_near(float z_near) { m_znear = z_near; }
void PixelCamera::set_far(float z_far) { m_zfar = z_far; }
float PixelCamera::znear() const { return m_znear; }
float PixelCamera::zfar() const { return m_zfar; }

glm::mat4 PixelCamera::projection() {
	return m_projection_matrix;
}

glm::mat4 PixelCamera::projection(Window& window) {
	update_projection(window);
	return m_projection_matrix;
}

Transformation& PixelCamera::transform() {
	return m_transformation;
}

glm::mat4 PixelCamera::view() {
	return m_transformation.matrix();
}

glm::mat4 PixelCamera::view(bool update) {
	if (update) update_view();
	return m_transformation.matrix();
}
};