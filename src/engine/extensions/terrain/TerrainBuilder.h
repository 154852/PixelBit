#pragma once

#include <vector>
#include "../../core/mesh/Mesh.h"

namespace PixelBit {
	namespace Terrain {
		class TerrainLayer {
		public:
			virtual float get(int x, int y, float seed, float existing) = 0;
		};

		class TerrainBuilder {
			std::vector<TerrainLayer*> m_layers;

			float permute_seed(float seed, int count);
		public:
			TerrainBuilder();

			void add(TerrainLayer* layer);

			float* create_height_matrix(int width, int height, float seed);
			float* create_vertices(int width, int height, float seed);
			void append_vbos(Mesh& mesh, int width, int height, float seed);
		};
	};
};