#include <iostream>
#include <chrono>
#include <thread>
#include <GL/glew.h>
#include "GL.h"
#include <functional>

namespace PixelBit {

GL::GL(Window& window): m_window(window), m_input(*(new Input(window))) {
}

void GL::update() {
	m_window.resized(true);
	m_window.update();
	m_input.update();
}

Input& GL::input() const {
	return m_input;
}

bool GL::closing() const {
	return m_window.closing();
}

Window& GL::window() const {
	return m_window;
}

bool GL::pressed(int key) const {
	return m_window.pressed(key);
}

GL& GL::viewport() {
	m_window.viewport();
	return *this;
}

GL& GL::wireframe() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	return *this;
}

GL& GL::depth(bool depthTest) {
	if (depthTest) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);
	return *this;
}

GL& GL::color(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
	return *this;
}

GL& GL::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	return *this;
}

GL& GL::animate(ApplicationLogic* callback) {
	return animate(true, callback);
}

GL& GL::animate(bool destroy, ApplicationLogic* callback) {
	m_running = true;
	while (m_running && !closing()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
		callback->animate(*this);
		update();
	}

	// if (destroy) GL::destroy();
	return *this;
}

GL& GL::animate(std::function<void (void)> callback) {
	return animate(true, callback);
}

GL& GL::animate(bool destroy, std::function<void (void)> callback) {
	m_running = true;
	while (m_running && !closing()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
		callback();
		update();
	}

	// if (destroy) GL::destroy();
	return *this;
}

GL& GL::stencil(bool only, bool inside, void (*write)(), void (*read)()) {
	glClearStencil(0);
	glStencilMask(0xFF);

	if (only) glColorMask(false, false, false, false);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	write();

	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	if (only) glColorMask(true, true, true, true);

	glStencilFunc(inside? GL_NOTEQUAL:GL_EQUAL, 1, 0xFF);
	read();

	glClear(GL_STENCIL_BUFFER_BIT);

	return *this;
}

GL& GL::lock_cursor() {
	glfwSetInputMode(m_window.handle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return *this;
}

GL& GL::unlock_cursor() {
	glfwSetInputMode(m_window.handle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	return *this;
}

GL& GL::create(const char* title, int width, int height) {
	if (!glfwInit()) throw std::runtime_error("Failed to initialise GLFW");
	Window& window = Window::create(width, height, title);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)  throw std::runtime_error("Failed to initialise GLEW");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GL* gl = new GL(window);
	gl->depth(true);
	return *gl;
}

GL& GL::create(const char* title) {
	return GL::create(title, 700, 400);
}

}