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
			for (int i = 0; i < m_layers.size(); i++) sum += m_layers.at(i)->get(x, y, permute_seed(seed, i), sum);
			heights[(x * width) + y] = sum;
			index++;
		}
	}

	return heights;
}

float* Terrain::TerrainBuilder::create_vertices(int width, int height, float seed) {
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

	return vertices;
}

void Terrain::TerrainBuilder::append_vbos(Mesh& mesh, int width, int height, float seed) {
	int index = 0;
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

	mesh.indices(indices, (width - 1) * (height - 1) * 2 * 3);
	mesh.vertex_buffer(create_vertices(width, height, seed), width * height * 3, 3);
}

};