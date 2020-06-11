#pragma once

#include "../TerrainBuilder.h"
#include "Simplex.h"

namespace PixelBit {

namespace Terrain {

class SimplexTerrainLayer: public TerrainLayer {
private:
	SimplexNoise m_noise;
	int m_octaves;
public:
	SimplexTerrainLayer(float frequency, float amplitude, int octaves=1, float lacunarity=2, float persistence=0.5f):
		m_noise(frequency, amplitude, lacunarity, persistence), m_octaves(octaves) {}

	virtual float get(int x, int y, float seed, float existing) override {
		return existing + m_noise.fractal(m_octaves, x, y, seed * 23934.2381);
	}
};

};

};