#pragma once

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>

namespace PixelBit {
	class Input;
	class Window;

	struct WindowPointer {
		Input* input;
		Window* window;
	};
	
	class Window {
	private:
		GLFWwindow* m_handle;
		int m_width;
		int m_height;
		GLFWvidmode m_vidmode;
		int m_device_pixel_ratio;
		bool m_resized { false };

		static void onResize(GLFWwindow* window, int newWidth, int newHeight);
		Window(GLFWwindow* handle, int width, int height);
	public:
		GLFWwindow* handle() const;
		int width() const;
		int height() const;
		int device_pixel_ratio() const;

		void move(double x, double y);

		void show();
		void hide();

		bool resized() const;
		bool resized(bool clear);

		void update();
		void viewport();

		bool closing() const;

		bool pressed(int keyCode) const;

		static Window& create(int width, int height, const char* title);
	};
}