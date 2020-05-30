#pragma once

#include "../../core/Window.h"
#include "Camera.h"

namespace PixelBit {
	class PixelCamera: public Camera {
	private:
		float m_znear { 0.01f };
		float m_zfar { 1000.0f };
		glm::mat4 m_projection_matrix;
		Transformation m_transformation;
	public:
		virtual glm::mat4 projection() override;
		virtual Transformation& transform() override;
		virtual glm::mat4 view() override;
		virtual void update_view() override;

		PixelCamera(Window& window);
		PixelCamera& update_projection(Window& window);

		void set_near(float z_near);
		void set_far(float z_far);
		float znear() const;
		float zfar() const;

		glm::mat4 projection(Window& window);
		glm::mat4 view(bool update);
	};
}