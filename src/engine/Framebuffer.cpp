#include <GL/glew.h>
#include "Framebuffer.h"

namespace PixelBit {

void Framebuffer::set_bound_fbo(int id) {
	static int bound = 0;
	if (bound != id) {
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id);
		bound = id;
	}
}

void Framebuffer::set_bound_rbo(int id) {
	static int bound = 0;
	if (bound != id) {
		glBindRenderbuffer(GL_RENDERBUFFER, id);
		bound = id;
	}
}

void Framebuffer::bind_fbo() { Framebuffer::set_bound_fbo(m_fbo_handle); }
void Framebuffer::bind_rbo() { Framebuffer::set_bound_rbo(m_rbo_handle); }
void Framebuffer::unbind_fbo() { Framebuffer::set_bound_fbo(0); }
void Framebuffer::unbind_rbo() { Framebuffer::set_bound_rbo(0); }

Framebuffer::Framebuffer(int width, int height) {
	m_width = width;
	m_height = height;
}

Framebuffer::Framebuffer(Window& window) {
	m_width = window.width() * window.device_pixel_ratio();
	m_height = window.height() * window.device_pixel_ratio();
}

Framebuffer& Framebuffer::color(bool enabled) {
	if (enabled) m_color = &Texture::empty(m_width, m_height);
	else m_color = NULL;
	return *this;
}

Framebuffer& Framebuffer::color(Texture& texture) {
	m_color = &texture;
	return *this;
}

Framebuffer& Framebuffer::depth(bool enabled) {
	if (enabled) m_depth = &Texture::empty(m_width, m_height, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT);
	else m_depth = NULL;
	return *this;
}

Framebuffer& Framebuffer::depth(Texture& texture) {
	m_depth = &texture;
	return *this;
}

Texture* Framebuffer::color() const { return m_color; }
Texture* Framebuffer::depth() const { return m_depth; }

Framebuffer& Framebuffer::compile() {
	if (m_color == NULL && m_depth == NULL) throw std::runtime_error("Cannot framebuffer with neither color nor depth.");

	glGenFramebuffers(1, (GLuint*) &m_fbo_handle); bind_fbo();
	glGenRenderbuffers(1, (GLuint*) &m_rbo_handle); bind_rbo();

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_COMPONENT, GL_RENDERBUFFER, m_rbo_handle);

	if (m_color != NULL) glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_color->handle(), 0);
	if (m_depth != NULL) glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, m_depth->handle(), 0);

	if (m_color != NULL && m_depth != NULL) glDrawBuffers(2, (GLenum[]) {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT});
	else if (m_color != NULL) glDrawBuffers(1, (GLenum[]) {GL_COLOR_ATTACHMENT0});
	else glDrawBuffers(1, (GLenum[]) {GL_DEPTH_ATTACHMENT});

	if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE_EXT) 
		throw new std::runtime_error("Failed to create framebuffer");
	
	Framebuffer::unbind_fbo();

	return *this;
}

Framebuffer& Framebuffer::resize(Window& window) {
	return resize(window.width() * window.device_pixel_ratio(), window.height() * window.device_pixel_ratio());
}

Framebuffer& Framebuffer::resize(int width, int height) {
	m_width = width; m_height = height;

	if (m_color != NULL) color(true);
	if (m_depth != NULL) depth(true);

	compile();

	bind_fbo();
	glViewport(0, 0, width, height);
	unbind_fbo();

	return *this;
}

Framebuffer& Framebuffer::draw_to(std::function<void(void)> callback) {
	bind_fbo();
	callback();
	unbind_fbo();

	return *this;
}

};