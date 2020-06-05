#pragma once

#include "../../core/GL.h"
#include "../../core/Transformation.h"

namespace PixelBit {

class Group: public Renderable, public Transformable {
private:
	std::vector<Renderable*> m_children;
	Transformation* m_transform;
public:
	Group();

	void add(Renderable*);
	virtual void render(glm::mat4* parent = NULL) override;
};

};