#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <GL/glew.h>
#include "Texture.h"

namespace PixelBit {

int Texture::prepare() {
	int textureId;
	glGenTextures(1, (GLuint*) &textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return textureId;
}

Texture& Texture::finalise(int id, int width, int height) {
	glGenerateMipmap(GL_TEXTURE_2D);
	Texture* tex = new Texture(id, width, height);
	return *tex;
}

Texture::Texture(int handle, int width, int height): m_handle(handle), m_width(width), m_height(height) {}

void Texture::bind() { Texture::set_bound_texture(m_handle); }
void Texture::unbind() { Texture::set_bound_texture(0); }

int Texture::handle() const { return m_handle; }

Texture& Texture::resize(Window& window) {
	return resize(window.width() * window.device_pixel_ratio(), window.height() * window.device_pixel_ratio());
}

Texture& Texture::resize(int width, int height) {
	m_width = width;
	m_height = height;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	return *this;
}

Texture& Texture::read(const char* filename) {
	int width, height, bytesPerPixel;
	unsigned char* imageData = stbi_load(filename, &width, &height, &bytesPerPixel, 0);

	int imageID = Texture::prepare();
	glTexImage2D(GL_TEXTURE_2D, 0, bytesPerPixel == 3? GL_RGB:GL_RGBA, width, height, 0, bytesPerPixel == 3? GL_RGB:GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	stbi_image_free(imageData);
	return Texture::finalise(imageID, width, height);
}

Texture& Texture::empty(int width, int height) {
	return empty(width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
}

Texture& Texture::empty(int width, int height, int encoding1, int encoding2, int type) {
	int imageID = Texture::prepare();
	glTexImage2D(GL_TEXTURE_2D, 0, encoding1, width, height, 0, encoding2, type, 0);
	return Texture::finalise(imageID, width, height);
}

void Texture::set_bound_texture(int id) {
	static int bound = 0;
	if (id != bound) {
		bound = 0;
		glBindTexture(GL_TEXTURE_2D, id);
	}
}

};