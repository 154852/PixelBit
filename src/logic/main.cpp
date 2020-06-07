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

class GameLogic: public PixelBit::ApplicationLogic {
private:
	PixelBit::Controls::FlightControls m_controls;
	PixelBit::LightScene::Scene m_scene;
public:
	GameLogic(PixelBit::GL& gl):
		m_scene(*(new PixelBit::PerspectiveCamera(gl.window()))),
		m_controls(PixelBit::Controls::FlightControls(gl)) {

		m_scene.camera()->update_projection(gl.window());

		// PixelBit::Loader::OBJLoader::load("assets/models/jet2/Airplane.obj", [this](PixelBit::Mesh* mesh, PixelBit::Loader::OBJLoader::OBJMaterial& material) {
		// 	PixelBit::LightScene::Material& mat = PixelBit::LightScene::Material::from("assets/models/jet2/", material);
		// 	mat.set_texture_transform(PixelBit::LightScene::TextureTransform::REVERSE_Y);
		// 	m_scene.add(new PixelBit::LightScene::SceneNode(mesh, mat));
		// });

		m_scene.add(new PixelBit::LightScene::PointLight(glm::vec3(0, 10, 0), 2.0f));

		PixelBit::Terrain::TerrainBuilder builder;
		builder.add(new PixelBit::Terrain::SimplexTerrainLayer(0.01f, 1.0f, 6));
		builder.add(new PixelBit::Terrain::MultiplierTerrainLayer(40.0f));

		PixelBit::Mesh* mesh = new PixelBit::Mesh();
		builder.append_vbos(*mesh, 1000, 1000, 0.1);
		
		PixelBit::LightScene::Material* material = new PixelBit::LightScene::Material();
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