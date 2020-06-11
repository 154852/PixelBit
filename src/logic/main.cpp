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
#include "../engine/extensions/terrain/BasicTerrainLayer.h"
#include "../engine/extensions/terrain/simplex/SimplexTerrainLayer.h"
#include "../engine/extensions/group/Group.h"

class GameLogic: public PixelBit::ApplicationLogic {
private:
	PixelBit::Controls::PerspectiveFly m_controls;
	PixelBit::LightScene::Scene m_scene;
public:
	GameLogic(PixelBit::GL& gl):
		m_scene(*(new PixelBit::PerspectiveCamera(gl.window()))),
		m_controls(PixelBit::Controls::PerspectiveFly(gl)) {

		m_controls.set_speed(0.5f);
		m_scene.camera()->update_projection(gl.window());

		PixelBit::Loader::OBJLoader::load("assets/models/jet2/Airplane.obj", [this](PixelBit::Mesh* mesh, PixelBit::Loader::OBJLoader::OBJMaterial& material) {
			PixelBit::LightScene::Material& mat = PixelBit::LightScene::Material::from("assets/models/jet2/", material);
			mat.set_texture_transform(PixelBit::LightScene::TextureTransform::REVERSE_Y);
			m_scene.add(new PixelBit::LightScene::SceneNode(mesh, mat));
		});

		m_scene.add(new PixelBit::LightScene::PointLight(glm::vec3(50, 10, 50), 0.0f));

		m_scene.directional_light().direction = glm::vec3(0, -1, 0);
		m_scene.set_ambient_light(glm::vec3(0.1f));


		PixelBit::Terrain::TerrainBuilder builder;
		builder.add(new PixelBit::Terrain::SimplexTerrainLayer(0.01f, 1.0f, 6));
		builder.add(new PixelBit::Terrain::MultiplierTerrainLayer(40.0f));

		PixelBit::Mesh* mesh = new PixelBit::Mesh();
		builder.append_vbos_smooth(*mesh, 100, 100, 0.1);
		PixelBit::LightScene::Material* material = new PixelBit::LightScene::Material();
		material->set_shininess(1);
		material->set_diffuse_color(glm::vec3(0.1, 0.9, 0.1));
		material->set_specular_color(glm::vec3(0.1, 0.9, 0.1));
		m_scene.add(new PixelBit::LightScene::SceneNode(mesh, *material));

		m_scene.compile();
		m_scene.camera()->transform()->translate(0, 0, 10);
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

	gl.animate(new GameLogic(gl));
}