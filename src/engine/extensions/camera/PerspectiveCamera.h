#pragma once

#include "../../core/Window.h"
#include "Camera.h"

namespace PixelBit {
	class PerspectiveCamera: public Camera {
	private:
		float m_znear { 0.01f };
		float m_zfar { 1000.0f };
		float m_fov { 1.047f };
		glm::mat4 m_projection_matrix;
	public:
		virtual glm::mat4 projection() override;
		virtual glm::mat4 view() override;
		virtual void update_view() override;

		PerspectiveCamera(Window& window);
		PerspectiveCamera& update_projection(Window& window);

		void set_near(float z_near);
		void set_far(float z_far);
		void set_fov(float fov);
		float znear() const;
		float zfar() const;
		float fov() const;

		glm::mat4 projection(Window& window);
		glm::mat4 view(bool update);
	};
};