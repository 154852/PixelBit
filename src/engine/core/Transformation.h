#pragma once
#include <glm/glm.hpp>

#include "Window.h"

namespace PixelBit {
	class Transformation {
	private:
		glm::vec3 m_position;
		glm::vec3 m_rotation;
		glm::vec3 m_scale;
		glm::mat4 m_model_matrix;
	public:
		Transformation(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
		Transformation();

		Transformation& update();
		Transformation& update_rotate_translate();
		Transformation& update_rotate_neg_translate();
		
		glm::mat4 matrix() const;
		glm::mat4 matrix(bool update);
		const glm::mat4* matrix_a() const;
		glm::mat4* matrix_a(bool update);
		
		glm::vec3 position() const;
		glm::vec3 rotation() const;
		glm::vec3 scale() const;

		Transformation& translate(float x, float y, float z);
		Transformation& position(float x, float y, float z);
		Transformation& position(glm::vec3 pos);

		Transformation& rotate(float x, float y, float z);
		Transformation& rotation(float x, float y, float z);
		Transformation& rotation(glm::vec3 rot);

		Transformation& scale(float xyz);
		Transformation& scale(float x, float y, float z);
		Transformation& scale(glm::vec3 sca);

		Transformation& center(Window& window);

		Transformation& forwards(glm::vec3 offset);
		Transformation& forwards(float x, float y, float z);

		glm::mat4 model_view(glm::mat4 view, bool update = true);
	};
};