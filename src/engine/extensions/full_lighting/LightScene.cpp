#include "LightScene.h"
#include "../../core/Utils.h"

namespace PixelBit {

LightScene::Material::Material() {}

Texture* LightScene::Material::diffuse() const { return m_diffuse; }
glm::vec3 LightScene::Material::diffuse_color() const { return m_diffuse_color; };
Texture* LightScene::Material::specular() const { return m_specular; } ;
glm::vec3 LightScene::Material::specular_color() const { return m_specular_color; };
float LightScene::Material::shininess() const { return m_shininess; };

LightScene::TextureTransform LightScene::Material::texture_transform() const { return m_texture_transform; };
void LightScene::Material::set_texture_transform(TextureTransform value) { m_texture_transform = value; }

void LightScene::Material::set_diffuse(Texture* value) { m_diffuse = value; }
void LightScene::Material::set_diffuse_color(glm::vec3 value) { m_diffuse_color = value; };
void LightScene::Material::set_specular(Texture* value) { m_specular = value; };
void LightScene::Material::set_specular_color(glm::vec3 value) { m_specular_color = value; };
void LightScene::Material::set_shininess(float value) { m_shininess = value; };

LightScene::Material& LightScene::Material::from(std::string textureSource, Loader::OBJLoader::OBJMaterial& material) {
	Material* out_material = new Material();
	if (material.diffuse_map->size() > 2) out_material->set_diffuse(&Texture::read((textureSource + *material.diffuse_map).c_str()));
	if (material.specular_map->size() > 2) out_material->set_specular(&Texture::read((textureSource + *material.specular_map).c_str()));
	if (material.specular_strength != NULL) out_material->set_shininess(*material.specular_strength);
	out_material->set_diffuse_color(*material.diffuse);
	out_material->set_specular_color(*material.specular);
	return *out_material;
}

void LightScene::Material::apply(Shader& shader) {
	shader.uniform<int>("material.diffuse", 0);
	shader.uniform<int>("material.specular", 1);
	shader.uniform<int>("texture_transform_mode", m_texture_transform);

	shader.uniform("material.diffuseColor", m_diffuse_color);
	if (m_diffuse != NULL) {
		shader.texture(*m_diffuse, 0);
		shader.uniform<int>("material.diffuseTextured", 1);
	} else shader.uniform<int>("material.diffuseTextured", 0);

	shader.uniform("material.specularColor", m_specular_color);
	if (m_specular != NULL) {
		shader.texture(*m_specular, 1);
		shader.uniform<int>("material.specularTextured", 1);
	} else shader.uniform<int>("material.specularTextured", 0);

	shader.uniform<float>("material.shininess", m_shininess);
}

LightScene::SceneNode::SceneNode(Renderable* mesh, Material& material):
	m_node(mesh), m_material(material), m_transform(*(new Transformation())) {}

void LightScene::SceneNode::set_shader(Shader *shader) { m_shader = shader; }
Shader* LightScene::SceneNode::shader() const { return m_shader; }

void LightScene::SceneNode::render(glm::mat4* parent) {
	m_material.apply(*m_shader);
	(*m_shader).uniform("model", parent == NULL? m_transform.matrix():((*parent) * m_transform.matrix()));
	m_node->render(parent);
}

Renderable* LightScene::SceneNode::renderable() const { return m_node; }
void LightScene::SceneNode::set_renderable(Renderable* mesh) { m_node = mesh; }

Transformation& LightScene::SceneNode::transform() { return m_transform; }
LightScene::Material& LightScene::SceneNode::material() { return m_material; }

void LightScene::DirectionalLight::apply(std::string name, Shader& shader) {
	shader
		.uniform(name + ".direction", direction)
		.uniform(name + ".ambient", ambient)
		.uniform(name + ".diffuse", diffuse)
		.uniform(name + ".specular", specular);
}

void LightScene::PointLight::apply(std::string name, Shader& shader) {
	shader
		.uniform(name + ".position", position)
		.uniform(name + ".ambient", ambient)
		.uniform(name + ".diffuse", diffuse)
		.uniform(name + ".specular", specular)
		.uniform<float>(name + ".constant", constant)
		.uniform<float>(name + ".linear", linear)
		.uniform<float>(name + ".quadratic", quadratic);
}

LightScene::PointLight::PointLight() {}
LightScene::PointLight::PointLight(glm::vec3 pos): position(pos) {}
LightScene::PointLight::PointLight(glm::vec3 pos, float intensity): position(pos), constant(intensity) {}

LightScene::Scene::Scene(PerspectiveCamera& camera): m_camera(camera), m_transform(new Transformation()) {}

Transformation* LightScene::Scene::transform() const { return m_transform; }

void LightScene::Scene::compile() {
	std::string replacement = std::to_string(m_point_lights.size());
	std::string fragment = read_file("assets/shaders/full_light/fragment.f.glsl");
	size_t index = 0;
	while (true) {
		index = fragment.find("NR_POINT_LIGHTS", index);
		if (index == std::string::npos) break;
		fragment.replace(index, 15, replacement);
		index += 15;
	}

	m_shader = &Shader::create(read_file("assets/shaders/full_light/vertex.v.glsl"), fragment);

	for (int i = 0; i < m_nodes.size(); i++) m_nodes[i]->set_shader(m_shader);
}

void LightScene::Scene::add(SceneNode* mesh) { m_nodes.push_back(mesh); }
void LightScene::Scene::add(LightScene::PointLight* light) { m_point_lights.push_back(light); }
PerspectiveCamera* LightScene::Scene::camera() { return &m_camera; }

void LightScene::Scene::render(glm::mat4* parent) {
	glm::mat4 newParent = parent == NULL? m_transform->matrix():((*parent) * m_transform->matrix());

	m_shader->uniform("projection", m_camera.projection())
		.uniform("view", m_camera.view(true))
		.uniform("viewPos", m_camera.transform().position());

	m_directional.apply("dirLight", *m_shader);

	for (int i = 0; i < m_point_lights.size(); i++) m_point_lights[i]->apply(concat("sis", "pointLights[", i, "]"), *m_shader);

	for (int i = 0; i < m_nodes.size(); i++) m_nodes[i]->render(&newParent);
}

};