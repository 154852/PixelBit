#include "FlightControls.h"
#include <iostream>
#include <cmath>

namespace PixelBit {
namespace Controls {
FlightControls::FlightControls(GL& gl): m_gl(gl) {
	enable();
	gl.input().listen(GLFW_KEY_ESCAPE, GLFW_PRESS, [this]() { toggle(); });

	std::cout << m_speed << std::endl;
}

FlightControls& FlightControls::toggle() {
	if (m_enabled) return disable();
	return enable();
}

FlightControls& FlightControls::enable() {
	m_enabled = true;
	m_gl.lock_cursor();
	return *this;
}

FlightControls& FlightControls::disable() {
	m_enabled = false;
	m_gl.unlock_cursor();
	return *this;
}

bool FlightControls::enabled() const { return m_enabled; }
float FlightControls::speed() const { return m_speed; }
FlightControls& FlightControls::set_speed(float value) {
	m_speed = value;

	return *this;
}

Transformation& FlightControls::update(Transformation& transformation) {
	if (!m_enabled || !m_gl.input().inside()) return transformation;

	if (m_gl.pressed(GLFW_KEY_LEFT_SHIFT)) m_speed += m_throttle_gradient;
	if (m_gl.pressed(GLFW_KEY_LEFT_CONTROL)) m_speed = fmaxf(m_speed - m_throttle_gradient, 0.0f);
	if (m_gl.pressed(GLFW_KEY_A)) transformation.relative_rotate_z(-m_roll_speed);
	if (m_gl.pressed(GLFW_KEY_D)) transformation.relative_rotate_z(m_roll_speed);
	if (m_gl.pressed(GLFW_KEY_W)) transformation.relative_rotate_x(m_pitch_speed);
	if (m_gl.pressed(GLFW_KEY_S)) transformation.relative_rotate_x(-m_pitch_speed);

	// std::cout << m_speed << std::endl;
	transformation.full_forwards(0, 0, -m_speed);

	return transformation;
}

void FlightControls::update(Camera* camera) {
	update(*camera->transform());
}

};
};
