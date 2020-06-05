#include "Utils.h"

#include <iostream>
#include <fstream>

namespace PixelBit {
std::string concat(const char* format...) {
	std::string all = "";

	va_list args;
	va_start(args, format);

	while (*format != '\0') {
		if (*format == 'i') all = all.append(std::to_string(va_arg(args, int)));
		else if (*format == 'c') all = all.append(1, static_cast<char>(va_arg(args, int)));
		else if (*format == 'f') all.append(std::to_string(va_arg(args, double)));
		else if (*format == 's') all = all.append(va_arg(args, const char*));
		++format;
	}

	va_end(args);
	return all;
}

bool write_to_file(const char* path, const char* data) {
	std::ofstream file; file.open(path);
	if (!file.is_open()) return false;
	file << data;
	file.close();
	return true;
}

std::string read_file(const char* path) {
	std::ifstream file(path);
	if (!file.is_open()) throw std::runtime_error(concat("sss", "File ", path, " does not exist"));
	std::string all;
	std::string line;
	while (std::getline(file, line)) all = all.append(line).append("\n");
	file.close();
	return all;
}

void debug_matrix(glm::mat4 matrix) {
	for (int i = 0; i < 4; i++)
		std::cout << "|" << matrix[0][i] << " " << matrix[1][i] << " " << matrix[2][i] << " " << matrix[3][i] << "|" << std::endl;
	std::cout << std::endl;
}

void debug_vector(glm::vec4 vector) {
	std::cout << "(" << vector[0] << ", " << vector[1] << ", " << vector[2] << ", " << vector[3] << ")" << std::endl;
}

void debug_vector(glm::vec3 vector) {
	std::cout << "(" << vector[0] << ", " << vector[1] << ", " << vector[2] << ")" << std::endl;
}
}