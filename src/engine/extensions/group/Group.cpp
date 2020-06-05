#include "Group.h"

namespace PixelBit {

Group::Group(): m_transform(new Transformation()) {}
void Group::add(Renderable* obj) { m_children.push_back(obj); }

void Group::render(glm::mat4* parent) {
	glm::mat4 newParent = parent == NULL? m_transform->matrix():((*parent) * m_transform->matrix());
	for (int i = 0; i < m_children.size(); i++) {
		Renderable* obj = m_children.at(i);
		obj->render(&newParent);
	}
}

};