#pragma once

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <functional>

#include "Input.h"
#include "Window.h"

namespace PixelBit {
	class GL;

	class ApplicationLogic {
	public:
		virtual void animate(GL& gl) = 0;
	};
	
	class GL {
	private:
		Window& m_window;
		bool m_running { false } ;
		Input& m_input;

	public:
		explicit GL(Window&);

		void update();
		bool closing() const;
		Window& window() const;
		Input& input() const;
		bool pressed(int key) const;
		GL& viewport();
		GL& wireframe();
		GL& depth(bool);
		GL& color(float r, float g, float b, float a);
		GL& clear();
		GL& animate(std::function<void (void)> callback);
		GL& animate(bool destroy, std::function<void (void)> callback);
		GL& animate(ApplicationLogic* logic);
		GL& animate(bool destroy, ApplicationLogic* logic);
		GL& stop();
		GL& stencil(bool only, bool inside, void (*write)(), void (*read)());
		GL& lock_cursor();
		GL& unlock_cursor();

		static GL& create(const char* title, int width, int height);
		static GL& create(const char* title);
	};
}