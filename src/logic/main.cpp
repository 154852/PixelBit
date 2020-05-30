#include <iostream>
#include <functional>

// #define SHADER_VALIDATE
#include "../engine/core/PixelBit.h"
#include "../engine/extensions/loaders/OBJLoader.h"
#include "../engine/extensions/controls/PerspectiveFly.h"
#include "../engine/extensions/camera/PerspectiveCamera.h"

class GameLogic: public PixelBit::ApplicationLogic {
private:
	PixelBit::PerspectiveCamera m_camera;
	std::vector<PixelBit::Mesh*> m_meshes;
	std::vector<PixelBit::Shader*> m_shaders;
	std::vector<PixelBit::Texture*> m_textures;
	PixelBit::Controls::PerspectiveFly m_controls;
public:
	GameLogic(PixelBit::GL& gl):
		m_camera(PixelBit::PerspectiveCamera(gl.window())),
		m_controls(PixelBit::Controls::PerspectiveFly(gl)) {

		m_controls.set_speed(1.0f);
		m_camera.set_far(10000);
		m_camera.update_projection(gl.window());

		PixelBit::Loader::OBJLoader::load("assets/models/jet2/Airplane.obj", [this](PixelBit::Mesh* mesh, PixelBit::Loader::OBJLoader::OBJMaterial& material) {
			PixelBit::Shader& shader = PixelBit::Shader::create("wavefront");
			shader.uniform("projectionMatrix", m_camera.projection());

			shader.uniform("scene.ambientLight.color", glm::vec3(1, 1, 1));
			shader.uniform<float>("scene.ambientLight.intensity", 0);

			shader.uniform("scene.pointLight.position", glm::vec3(8, 19, 9));
			shader.uniform("scene.pointLight.color", glm::vec3(1, 1, 1));
			shader.uniform<float>("scene.pointLight.intensity", 3);
			shader.uniform<float>("scene.pointLight.specularStrength", 32);

			shader.uniform("material.diffuse", *material.diffuse);
			shader.uniform<int>("forceFlatShade", 0);

			if ((*material.diffuse_map).length() > 1) {
				shader.uniform<int>("textured", 1);
				shader.uniform<int>("diffuse_map", 0);

				PixelBit::Texture& texture = PixelBit::Texture::read(std::string("assets/models/jet2/").append(*material.diffuse_map).c_str());
				m_textures.push_back(&texture);
			} else {
				shader.uniform<int>("textured", 0);
				m_textures.push_back(0);
			}

			m_meshes.push_back(mesh);
			m_shaders.push_back(&shader);
		});

		m_camera.transform().translate(0, 0, 10);
	}

	virtual void animate(PixelBit::GL& gl) override {
		if (gl.window().resized(false)) {
			gl.viewport();
			m_camera.update_projection(gl.window());
		}
		gl.clear();

		m_controls.update(&m_camera);
		m_camera.view(true);

		for (int i = 0; i < m_meshes.size(); i++) {
			PixelBit::Mesh* mesh = m_meshes.at(i);
			PixelBit::Shader* shader = m_shaders.at(i);
			PixelBit::Texture* texture = m_textures.at(i);

			shader->uniform("modelViewMatrix", m_camera.view(true));
			shader->uniform("cameraPosition", m_camera.transform().position());

			shader->uniform("projectionMatrix", m_camera.projection());

			if (texture != NULL) shader->texture(*texture, 0);
			shader->render(*mesh);
		}
	};
};

int main() {
	auto gl = PixelBit::GL::create("Hello World!");
	gl.color(0.5, 0.5, 0.5, 1);

	gl.animate(new GameLogic(gl));
}