#include <iostream>
#include <functional>
#include <cmath>

#define SHADER_VALIDATE
#include "../engine/core/PixelBit.h"
#include "../engine/extensions/loaders/OBJLoader.h"
#include "../engine/extensions/controls/PerspectiveFly.h"
#include "../engine/extensions/controls/FlightControls.h"
#include "../engine/extensions/camera/PerspectiveCamera.h"
#include "../engine/extensions/full_lighting/LightScene.h"

class GameLogic: public PixelBit::ApplicationLogic {
private:
	PixelBit::Controls::FlightControls m_controls;
	PixelBit::LightScene::Scene m_scene;
public:
	GameLogic(PixelBit::GL& gl):
		m_scene(*(new PixelBit::PerspectiveCamera(gl.window()))),
		m_controls(PixelBit::Controls::FlightControls(gl)) {

		// m_controls.set_speed(1.0f);
		m_scene.camera()->update_projection(gl.window());

		PixelBit::Loader::OBJLoader::load("assets/models/jet2/Airplane.obj", [this](PixelBit::Mesh* mesh, PixelBit::Loader::OBJLoader::OBJMaterial& material) {
			PixelBit::LightScene::Material& mat = PixelBit::LightScene::Material::from("assets/models/jet2/", material);
			mat.set_texture_transform(PixelBit::LightScene::TextureTransform::REVERSE_Y);
			m_scene.add(new PixelBit::LightScene::SceneNode(mesh, mat));
		});

		m_scene.add(new PixelBit::LightScene::PointLight(glm::vec3(0, 10, 0), 2.0f));

		m_scene.compile();
		m_scene.camera()->transform()->translate(0, 0, 10);

		m_scene.transform()->set_euler(0, 0, 3.14159f / 2.0f).update();
	}

	virtual void animate(PixelBit::GL& gl) override {
		if (gl.window().resized(false)) {
			gl.viewport();
			m_scene.camera()->update_projection(gl.window());
		}
		gl.clear();


		m_controls.update(m_scene.camera());
		m_scene.render();
	};
};

int main() {
	auto gl = PixelBit::GL::create("Hello World!");
	gl.color(0.2, 0.2, 0.2, 1);

	PixelBit::Transformation transform;
	transform.relative_rotate_x(M_PI / 2);
	PixelBit::debug_matrix(transform.matrix(true));

	gl.animate(new GameLogic(gl));
}