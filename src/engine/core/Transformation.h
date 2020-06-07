#pragma once

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>

#include "Window.h"

namespace PixelBit {
	class Transformation {
	private:
		glm::vec3 m_position;
		glm::vec3 m_scale;
		glm::mat4 m_model_matrix;
		glm::quat m_rotation_q;
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
		glm::quat rotation();
		glm::vec3 rotation_euler() const;
		glm::vec3 scale() const;

		Transformation& translate(float x, float y, float z);
		Transformation& position(float x, float y, float z);
		Transformation& position(glm::vec3 pos);

		Transformation& add_euler(float x, float y, float z);
		Transformation& set_euler(float x, float y, float z);
		Transformation& rotation(glm::quat rot);

		Transformation& scale(float xyz);
		Transformation& scale(float x, float y, float z);
		Transformation& scale(glm::vec3 sca);

		Transformation& center(Window& window);

		Transformation& forwards(glm::vec3 offset);
		Transformation& forwards(float x, float y, float z);
		Transformation& full_forwards(glm::vec3 offset);
		Transformation& full_forwards(float x, float y, float z);
		Transformation& full_forwards(glm::vec3 offset, glm::vec3 axis_multiplier);

		Transformation& relative_rotate(glm::vec3 axis, float angle);
		Transformation& relative_rotate_x(float angle);
		Transformation& relative_rotate_y(float angle);
		Transformation& relative_rotate_z(float angle);
		Transformation& world_rotate(glm::vec3 axis, float angle);
		Transformation& world_rotate_x(float angle);
		Transformation& world_rotate_y(float angle);
		Transformation& world_rotate_z(float angle);

		glm::mat4 model_view(glm::mat4 view, bool update = true);
	};

	class Transformable {
	protected:
		Transformation* m_transform;
	public:
		Transformation* transform() const { return m_transform; }
	};
};