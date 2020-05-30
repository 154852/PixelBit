#pragma once

#include <glm/glm.hpp>
#include "../Transformation.h"

namespace PixelBit {
	class Camera {
	public:
		virtual glm::mat4 projection() = 0;
		virtual Transformation& transform() = 0;
		virtual glm::mat4 view() = 0;
		virtual void update_view() = 0;
	};
};