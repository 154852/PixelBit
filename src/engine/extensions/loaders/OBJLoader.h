#pragma once

#include <vector>
#include <functional>
#include "../../core/mesh/Mesh.h"
#include "../../core/shader/Shader.h"
#include <string>

namespace PixelBit {
	namespace Loader {
		namespace OBJLoader {
			struct OBJMaterial {
				glm::vec3* ambient { NULL };
				glm::vec3* diffuse { NULL };
				glm::vec3* specular { NULL };
				float* specular_strength { NULL };
				float* optical_density { NULL };
				float* disolve { NULL };
				int* illumination { NULL };
				std::string* ambient_map { NULL };
				std::string* diffuse_map { NULL };
				std::string* specular_map { NULL };
				std::string* specular_highlight_map { NULL };
				std::string* alpha_map { NULL };
				std::string* height_map { NULL };
			};

			extern void load(const char* path, std::function<void(Mesh*, OBJMaterial&)> shaderGen);
			extern void load(const char* path, std::vector<Mesh*>& meshes);
		};
	};
};