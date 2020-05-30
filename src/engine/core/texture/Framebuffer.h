#pragma once

#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <iostream>
#include "Texture.h"
#include "../Window.h"
#include <functional>

namespace PixelBit {
	class Framebuffer {
	private:
		bool m_active { true };
		int m_fbo_handle;
		int m_rbo_handle;

		Texture* m_color { NULL };
		Texture* m_depth { NULL };

		int m_width;
		int m_height;

		static void set_bound_fbo(int id);
		static void set_bound_rbo(int id);
	public:
		Framebuffer(Window& window);
		Framebuffer(int width, int height);
		Framebuffer& color(bool enabled);
		Framebuffer& color(Texture& texture);
		Framebuffer& depth(bool enabled);
		Framebuffer& depth(Texture& texture);
		Texture* color() const;
		Texture* depth() const;
		Framebuffer& compile();
		Framebuffer& resize(Window& window);
		Framebuffer& resize(int width, int height);
		void bind_fbo();
		void bind_rbo();
		Framebuffer& draw_to(std::function<void(void)> callback);

		static void unbind_fbo();
		static void unbind_rbo();
	};
}