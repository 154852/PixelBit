#pragma once

#include <iostream>
#include "Window.h"

namespace PixelBit {
	class Texture {
	private:
		int m_handle;
		int m_width;
		int m_height;

		static void set_bound_texture(int id);

		static int prepare();
		static Texture& finalise(int id, int width, int height);
	public:
		Texture(int handle, int width, int height);
		void bind();
		int handle() const;

		Texture& resize(Window& window);
		Texture& resize(int width, int height);

		static Texture& read(const char* filename);
		static Texture& empty(int width, int height);
		static Texture& empty(int width, int height, int encoding1, int encoding2, int type);

		static void unbind();
	};
}