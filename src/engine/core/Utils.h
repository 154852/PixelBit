#pragma once

#include <string>
#include <glm/glm.hpp>

namespace PixelBit {
	extern std::string concat(const char* format...);
	extern bool write_to_file(const char* path, const char* data);
	extern std::string read_file(const char* path);
	extern void debug_matrix(glm::mat4 matrix);
}