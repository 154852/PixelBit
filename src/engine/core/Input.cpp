#include "Input.h"

namespace PixelBit {

KeyListener::KeyListener(int key, int action, std::function<void(void)> callback): m_key(key), m_action(action), m_callback(callback) {}
bool KeyListener::fits(int key, int action) { return key == m_key && action == m_action; }
void KeyListener::run() { m_callback(); }

ButtonListener::ButtonListener(int key, int action, std::function<void(void)> callback): m_button(key), m_action(action), m_callback(callback) {}
bool ButtonListener::fits(int key, int action) { return key == m_button && action == m_action; }
void ButtonListener::run() { m_callback(); }

void Input::cursor_pos_callback(GLFWwindow* window, double x, double y) {
	Input* input = ((WindowPointer*) glfwGetWindowUserPointer(window))->input;
	input->m_cursor.x = (float) x;
	input->m_cursor.y = (float) y;
}

void Input::cursor_enter_callback(GLFWwindow* window, int entered) {
	Input* input = ((WindowPointer*) glfwGetWindowUserPointer(window))->input;
	input->m_inside = entered;
}

void Input::scroll_callback(GLFWwindow* window, double x, double y) {
	Input* input = ((WindowPointer*) glfwGetWindowUserPointer(window))->input;
	input->m_scroll_motion = (float) y;
}

void Input::mouse_button_callback(GLFWwindow* window, int button, int action, int mode) {
	Input* input = ((WindowPointer*) glfwGetWindowUserPointer(window))->input;
	
	input->m_mouse_left = button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS;
	input->m_mouse_right = button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS;

	if (action != GLFW_REPEAT) {
		for (ButtonListener listener : input->m_button_listeners) {
			if (listener.fits(button, action)) listener.run();
		}
	}
}

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Input* input = ((WindowPointer*) glfwGetWindowUserPointer(window))->input;
	
	if (!input->m_inside) return;
	input->m_keys[key] = action == GLFW_REPEAT? GLFW_PRESS:action;

	if (action != GLFW_REPEAT) {
		for (KeyListener listener : input->m_key_listeners) {
			if (listener.fits(key, action)) listener.run();
		}
	}
}

Input::Input(Window& window) {
	double x; double y; glfwGetCursorPos(window.handle(), &x, &y);
	m_cursor.x = (float) x; m_cursor.y = (float) y;
	m_cursor_last.x = m_cursor.x; m_cursor_last.y = m_cursor.y;

	WindowPointer* ptr = (WindowPointer*) malloc(sizeof(WindowPointer));
	ptr->window = &window;
	ptr->input = this;
	glfwSetWindowUserPointer(window.handle(), ptr);

	glfwSetCursorPosCallback(window.handle(), Input::cursor_pos_callback);
	glfwSetCursorEnterCallback(window.handle(), Input::cursor_enter_callback);
	glfwSetScrollCallback(window.handle(), Input::scroll_callback);

	glfwSetMouseButtonCallback(window.handle(), Input::mouse_button_callback);
	glfwSetKeyCallback(window.handle(), Input::key_callback);
}

KeyListener& Input::key_listen(int key, int action, std::function<void(void)> callback) {
	KeyListener* listener = new KeyListener(key, action, callback);
	m_key_listeners.push_back(*listener);
	return *listener;
}

ButtonListener& Input::button_listen(int button, int action, std::function<void(void)> callback) {
	ButtonListener* listener = new ButtonListener(button, action, callback);
	m_button_listeners.push_back(*listener);
	return *listener;
}

void Input::listen(int button, int action, std::function<void(void)> callback) {
	if (button <= 2) button_listen(button, action, callback);
	else key_listen(button, action, callback);
}

void Input::remove(KeyListener& listener) {
	// TODO: Remove key listener
}

void Input::remove(ButtonListener& listener) {
	// TODO: Remove button listener
}

void Input::update() {
	if (m_inside) {
		m_cursor_motion = m_cursor - m_cursor_last;
		m_cursor_last.x = m_cursor.x; m_cursor_last.y = m_cursor.y;
		m_scroll += m_scroll_motion;
	} else {
		m_cursor_motion.x = 0;
		m_cursor_motion.y = 0;
	}

	m_scroll_motion = 0;
}

bool Input::inside() const { return m_inside; }
bool Input::mouse_left() const { return m_mouse_left; }
bool Input::mouse_right() const { return m_mouse_right; }
glm::vec2 Input::cursor() const { return m_cursor; }
glm::vec2 Input::cursor_motion() const { return m_cursor_motion; }
float Input::scroll() const { return m_scroll; }
float Input::scroll_motion() const { return m_scroll_motion; }

bool Input::pressed(int code) {
	return m_keys.count(code) && (m_keys[code] == GLFW_PRESS);
}

};