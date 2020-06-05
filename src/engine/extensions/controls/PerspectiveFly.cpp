#include "PerspectiveFly.h"
#include <iostream>

namespace PixelBit {
namespace Controls {
PerspectiveFly::PerspectiveFly(GL& gl): m_gl(gl) {
	enable();
	gl.input().listen(GLFW_KEY_ESCAPE, GLFW_PRESS, [this]() { toggle(); });
}

PerspectiveFly& PerspectiveFly::toggle() {
	if (m_enabled) return disable();
	return enable();
}

PerspectiveFly& PerspectiveFly::enable() {
	m_enabled = true;
	m_gl.lock_cursor();
	return *this;
}

PerspectiveFly& PerspectiveFly::disable() {
	m_enabled = false;
	m_gl.unlock_cursor();
	return *this;
}

bool PerspectiveFly::enabled() const { return m_enabled; }
float PerspectiveFly::speed() const { return m_speed; }
PerspectiveFly& PerspectiveFly::set_speed(float value) {
	m_speed = value;
	return *this;
}
float PerspectiveFly::rotation_sensitivity() const { return m_rotation_sensitivity; }
PerspectiveFly& PerspectiveFly::set_rotation_sensitivity(float value) {
	m_rotation_sensitivity = value;
	return *this;
}


Transformation& PerspectiveFly::update(Transformation& transformation) {
	if (!m_enabled || !m_gl.input().inside()) return transformation;

	glm::vec3 translation(0);
	Window& window = m_gl.window();
	if (window.pressed(GLFW_KEY_W)) translation.z -= 1;
	if (window.pressed(GLFW_KEY_S)) translation.z += 1;
	if (window.pressed(GLFW_KEY_A)) translation.x -= 1;
	if (window.pressed(GLFW_KEY_D)) translation.x += 1;
	if (window.pressed(GLFW_KEY_LEFT_SHIFT)) translation.y -= 1;
	if (window.pressed(GLFW_KEY_SPACE)) translation.y += 1;

	if (glm::length(translation) != 0) transformation.forwards(glm::normalize(translation) * m_speed);

	glm::vec2 pos = m_gl.input().cursor_motion();
	transformation.add_euler(pos.y * m_rotation_sensitivity, pos.x * m_rotation_sensitivity, 0);

	return transformation;
}

void PerspectiveFly::update(Camera* camera) {
	update(*camera->transform());
}

};
};
