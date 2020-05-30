#include "Window.h"
#include <iostream>
#include <functional>

#include <GLFW/glfw3.h>

namespace PixelBit {

Window::Window(GLFWwindow* handle, int width, int height) {
	m_handle = handle;
	m_width = width;
	m_height = height;

	m_vidmode = *glfwGetVideoMode(glfwGetPrimaryMonitor());

	int frameWidth;
	glfwGetFramebufferSize(m_handle, &frameWidth, 0);
	m_device_pixel_ratio = frameWidth / m_width;

	WindowPointer* ptr = (WindowPointer*) malloc(sizeof(WindowPointer));
	ptr->window = this;
	ptr->input = NULL;
	glfwSetWindowUserPointer(m_handle, ptr);
	glfwSetWindowSizeCallback(m_handle, onResize);
}

void Window::onResize(GLFWwindow* handle, int newWidth, int newHeight) {
	Window* window = ((WindowPointer*) glfwGetWindowUserPointer(handle))->window;
	window->m_width = newWidth;
	window->m_height = newHeight;
	window->m_resized = true;
}

void Window::move(double x, double y) {
	glfwSetWindowPos(
		m_handle,
		(int) ((m_vidmode.width - m_width) * x),
		(int) ((m_vidmode.height - m_height) * y)
	);
}

GLFWwindow* Window::handle() const { return m_handle; }

int Window::width() const { return m_width; }
int Window::height() const { return m_height; }
int Window::device_pixel_ratio() const { return m_device_pixel_ratio; }

void Window::show() { glfwShowWindow(m_handle); }
void Window::hide() { glfwHideWindow(m_handle); }
bool Window::resized() const { return m_resized; }

bool Window::resized(bool clear) {	
	if (clear) {
		if (m_resized) {
			m_resized = false;
			return true;
		}
		return false;
	}
	return m_resized;
}

void Window::update() {
	glfwPollEvents();
	glfwSwapBuffers(m_handle);
}

void Window::viewport() {
	glViewport(0, 0, m_width * m_device_pixel_ratio, m_height * m_device_pixel_ratio);
}

bool Window::closing() const {
	return glfwWindowShouldClose(m_handle);
}

bool Window::pressed(int keyCode) const {
	return glfwGetKey(m_handle, keyCode) == GLFW_PRESS;
}

Window& Window::create(int width, int height, const char* title) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#if __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

	GLFWwindow* handle = glfwCreateWindow(width, height, title, 0, 0);

	if (handle == NULL) {
		throw std::runtime_error("Failed to open GLFW window");
	}
	glfwMakeContextCurrent(handle);

	Window* window = new Window(handle, width, height);

	glfwSwapInterval(1);

	window->show();
	return *window;
}

}