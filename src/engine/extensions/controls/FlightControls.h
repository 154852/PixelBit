#pragma once
#include "../../core/GL.h"
#include "../../core/Transformation.h"
#include "../camera/Camera.h"

namespace PixelBit {
	namespace Controls {
		class FlightControls {
			private:
				GL& m_gl;
				float m_speed { 0.0f };
				float m_throttle_gradient { 0.1 };
				float m_roll_speed { 0.05f };
				float m_pitch_speed { 0.05f };
				float m_enabled {false};
			public:
				explicit FlightControls(GL& gl);

				FlightControls& toggle();
				FlightControls& enable();
				FlightControls& disable();
				bool enabled() const;

				float speed() const;
				FlightControls& set_speed(float value);

				Transformation& update(Transformation& transformation);
				void update(Camera* camera);
		};
	}
};