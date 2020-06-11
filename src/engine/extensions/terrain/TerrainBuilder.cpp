#include "TerrainBuilder.h"
#include <iostream>

namespace PixelBit {

Terrain::TerrainBuilder::TerrainBuilder() {}

void Terrain::TerrainBuilder::add(Terrain::TerrainLayer* layer) {
	m_layers.push_back(layer);
}

float Terrain::TerrainBuilder::permute_seed(float seed, int count) {
	return fmod((double) seed * (count + 1.0f) * 7394.1394, 1.0f);
}

float* Terrain::TerrainBuilder::create_height_matrix(int width, int height, float seed) {
	float* heights = (float*) calloc(width * height, sizeof(float));

	int index = 0;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			float sum = 0;
			for (int i = 0; i < m_layers.size(); i++) sum = m_layers.at(i)->get(x, y, permute_seed(seed, i), sum);
			heights[index] = sum;
			index++;
		}
	}

	return heights;
}

void Terrain::TerrainBuilder::append_vbos_smooth(Mesh& mesh, int width, int height, float seed) {
	float* heights = create_height_matrix(width, height, seed);
	float* vertices = (float*) calloc(width * height * 3, sizeof(float));

	int index = 0;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			vertices[index++] = x;
			vertices[index++] = heights[(x * width) + y];
			vertices[index++] = y;
		}
	}

	index = 0;
	int* indices = (int*) calloc((width - 1) * (height - 1) * 2 * 3, sizeof(int));

	for (int x = 0; x < width - 1; x++) {
		for (int y = 0; y < height - 1; y++) {
			int xy = (x * width) + y;
			int x1y = ((x + 1) * width) + y;
			int xy1 = (x * width) + y + 1;
			int x1y1 = ((x + 1) * width) + y + 1;

			indices[index++] = xy;
			indices[index++] = x1y;
			indices[index++] = xy1;

			indices[index++] = xy1;
			indices[index++] = x1y;
			indices[index++] = x1y1;
		}
	}

	float* normals = (float*) calloc(width * height * 3, sizeof(float));
	for (int i = 0; i < width * height * 3; i++) normals[i] = 0;

	for (index = 0; index < (width - 1) * (height - 1) * 2; index++) {
		glm::ivec3 face(indices[index * 3], indices[(index * 3) + 1], indices[(index * 3) + 2]);
		glm::vec3 p0(vertices[face.x * 3], vertices[(face.x * 3) + 1], vertices[(face.x * 3) + 2]);
		glm::vec3 p1(vertices[face.y * 3], vertices[(face.y * 3) + 1], vertices[(face.y * 3) + 2]);
		glm::vec3 p2(vertices[face.z * 3], vertices[(face.z * 3) + 1], vertices[(face.z * 3) + 2]);

		glm::vec3 face_normal = glm::cross(p1 - p0, p2 - p0);
		normals[face.x * 3] += face_normal.x; normals[(face.x * 3) + 1] += face_normal.y; normals[(face.x * 3) + 2] += face_normal.z;
		normals[face.y * 3] += face_normal.x; normals[(face.y * 3) + 1] += face_normal.y; normals[(face.y * 3) + 2] += face_normal.z;
		normals[face.z * 3] += face_normal.x; normals[(face.z * 3) + 1] += face_normal.y; normals[(face.z * 3) + 2] += face_normal.z;
	}

	for (int i = 0; i < width * height * 3; i += 3) {
		glm::vec3 norm(normals[i], normals[i + 1], normals[i + 2]);
		norm = glm::normalize(norm);
		normals[i] = norm.x; normals[i + 1] = norm.y; normals[i + 2] = norm.z;
	}

	mesh.indices(indices, (width - 1) * (height - 1) * 2 * 3);
	mesh.vertex_buffer(vertices, width * height * 3, 3);
	mesh.vertex_buffer(normals, width * height * 3, 3);
}

void Terrain::TerrainBuilder::append_vbos_flat(Mesh& mesh, int width, int height, float seed) {
	int idx_index = 0;
	int triangles = (width - 1) * (height - 1) * 2;
	int* indices = (int*) calloc(triangles * 3, sizeof(int));

	int vert_index = 0;
	float* vertices = (float*) calloc(triangles * 3 * 3, sizeof(float));
	float* heights = create_height_matrix(width, height, seed);

	int normal_index = 0;
	float* normals = (float*) calloc(triangles * 3 * 3, sizeof(float));

	for (int x = 0; x < width - 1; x++) {
		for (int y = 0; y < height - 1; y++) {
			int xy = (x * width) + y;
			int x1y = ((x + 1) * width) + y;
			int xy1 = (x * width) + y + 1;
			int x1y1 = ((x + 1) * width) + y + 1;

			glm::vec3 p2(x, heights[xy], y);
			glm::vec3 p1(x + 1, heights[x1y], y);
			glm::vec3 p0(x, heights[xy1], y + 1);
			glm::vec3 face_normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));

			indices[idx_index] = idx_index; idx_index++;
			vertices[vert_index++] = x; vertices[vert_index++] = heights[xy]; vertices[vert_index++] = y;
			normals[normal_index++] = face_normal.x; normals[normal_index++] = face_normal.y; normals[normal_index++] = face_normal.z;

			indices[idx_index] = idx_index; idx_index++;
			vertices[vert_index++] = x + 1; vertices[vert_index++] = heights[x1y]; vertices[vert_index++] = y;
			normals[normal_index++] = face_normal.x; normals[normal_index++] = face_normal.y; normals[normal_index++] = face_normal.z;

			indices[idx_index] = idx_index; idx_index++;
			vertices[vert_index++] = x; vertices[vert_index++] = heights[xy1]; vertices[vert_index++] = y + 1;
			normals[normal_index++] = face_normal.x; normals[normal_index++] = face_normal.y; normals[normal_index++] = face_normal.z;

			p2 = glm::vec3(x, heights[xy1], y + 1);
			p0 = glm::vec3(x + 1, heights[x1y1], y + 1);
			face_normal = glm::normalize(glm::cross(p1 - p0, p2 - p0));

			indices[idx_index] = idx_index; idx_index++;
			vertices[vert_index++] = x; vertices[vert_index++] = heights[xy1]; vertices[vert_index++] = y + 1;
			normals[normal_index++] = face_normal.x; normals[normal_index++] = face_normal.y; normals[normal_index++] = face_normal.z;

			indices[idx_index] = idx_index; idx_index++;
			vertices[vert_index++] = x + 1; vertices[vert_index++] = heights[x1y]; vertices[vert_index++] = y;
			normals[normal_index++] = face_normal.x; normals[normal_index++] = face_normal.y; normals[normal_index++] = face_normal.z;

			indices[idx_index] = idx_index; idx_index++;
			vertices[vert_index++] = x + 1; vertices[vert_index++] = heights[x1y1]; vertices[vert_index++] = y + 1;
			normals[normal_index++] = face_normal.x; normals[normal_index++] = face_normal.y; normals[normal_index++] = face_normal.z;
		}
	}

	mesh.indices(indices, triangles * 3);
	mesh.vertex_buffer(vertices, triangles * 3 * 3, 3);
	mesh.vertex_buffer(normals, triangles * 3 * 3, 3);
}

};