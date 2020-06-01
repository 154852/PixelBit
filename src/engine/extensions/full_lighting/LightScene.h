#pragma once

#include "../loaders/OBJLoader.h"
#include "../camera/PerspectiveCamera.h"
#include "../../core/texture/Texture.h"
#include "../../core/mesh/Mesh.h"
#include "../../core/Transformation.h"
#include "../../core/shader/Shader.h"
#include <glm/glm.hpp>
#include <vector>

namespace PixelBit {
	namespace LightScene {
		enum TextureTransform {
			DEFAULT,
			REVERSE_X,
			REVERSE_Y,
			REVERSE_XY
		};

		class Material {
		private:
			Texture* m_diffuse { NULL };
			glm::vec3 m_diffuse_color {1};
			Texture* m_specular { NULL };
			glm::vec3 m_specular_color {1};
			float m_shininess {32.0f};
			TextureTransform m_texture_transform { TextureTransform::DEFAULT };
		public:
			Material();

			Texture* diffuse() const;
			glm::vec3 diffuse_color() const;
			Texture* specular() const;
			glm::vec3 specular_color() const;
			float shininess() const;

			TextureTransform texture_transform() const;
			void set_texture_transform(TextureTransform);

			void set_diffuse(Texture*);
			void set_diffuse_color(glm::vec3);
			void set_specular(Texture*);
			void set_specular_color(glm::vec3);
			void set_shininess(float);

			void apply(Shader& shader);
			static Material& from(std::string textureSource, Loader::OBJLoader::OBJMaterial& material);
		};

		class SceneNode {
		private:
			Mesh& m_mesh;
			Transformation& m_transform;
			Material& m_material;
		public:
			SceneNode(Mesh& mesh, Material& material);
			void render(Shader& shader);

			Mesh& mesh() const;
			void set_mesh(Mesh& mesh);

			Transformation& transform();
			Material& material();
		};

		class DirectionalLight {
		public:
			glm::vec3 direction {-0.2f, -1.0f, -0.3f};
		    glm::vec3 ambient {0.05f};
		    glm::vec3 diffuse {0.4f};
		    glm::vec3 specular {0.5f};

		    void apply(std::string name, Shader& shader);
		};
		// class SpotLight {};
		class PointLight {
		public:
			glm::vec3 position {0};
    
		    float constant { 1.0f };
		    float linear { 0.09f };
		    float quadratic { 0.032f };
			
		    glm::vec3 ambient {0.05f};
		    glm::vec3 diffuse {0.8f};
		    glm::vec3 specular {1.0f};

		    void apply(std::string name, Shader& shader);

		    PointLight();
		    PointLight(glm::vec3 pos);
		    PointLight(glm::vec3 pos, float intensity);
		};

		class Scene {
		private:
			std::vector<SceneNode*> m_nodes;
			DirectionalLight m_directional;
			std::vector<PointLight*> m_point_lights;
			Shader* m_shader;
			PerspectiveCamera m_camera;
		public:
			Scene(PerspectiveCamera& camera);
			void add(SceneNode* mesh);
			void add(PointLight* mesh);
			void compile();
			void render();

			PerspectiveCamera* camera();
		};
	};
};