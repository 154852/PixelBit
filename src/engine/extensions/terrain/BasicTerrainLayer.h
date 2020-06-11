#pragma once

#include "TerrainBuilder.h"

namespace PixelBit {

namespace Terrain {

class FlatTerrainLayer: public TerrainLayer {
private:
	float m_constant;
public:
	FlatTerrainLayer(float constant = 0): m_constant(constant) {}

	virtual float get(int x, int y, float seed, float existing) override {
		return existing + m_constant;
	}

	float constant() const { return m_constant; };
	void set_constant(float constant) { m_constant = constant; };
};

class MultiplierTerrainLayer: public TerrainLayer {
private:
	float m_constant;
public:
	MultiplierTerrainLayer(float constant = 0): m_constant(constant) {}

	virtual float get(int x, int y, float seed, float existing) override {
		return existing * m_constant;
	}

	float constant() const { return m_constant; };
	void set_constant(float constant) { m_constant = constant; };
};

};

};