#pragma once
#include "../GL.h"
#include "../Transformation.h"
#include "../camera/Camera.h"

namespace PixelBit {
	namespace Controls {
		class PerspectiveFly {
			private:
				GL& m_gl;
				float m_speed { 0.2f };
				float m_rotation_sensitivity { 0.002f };
				float m_enabled {false};
			public:
				explicit PerspectiveFly(GL& gl);

				PerspectiveFly& toggle();
				PerspectiveFly& enable();
				PerspectiveFly& disable();
				bool enabled() const;

				float speed() const;
				PerspectiveFly& set_speed(float value);
				float rotation_sensitivity() const;
				PerspectiveFly& set_rotation_sensitivity(float value);

				Transformation& update(Transformation& transformation);
				void update(Camera* camera);
		};
	}
};