#pragma once
#include <vector>
#include "Plate.h"
class Lithosphere
{
public:
	Lithosphere();

	int width;
	int height;
	std::vector < std::vector<float>> lithoHeightMap;
	std::vector<Plate> plates;

	void GenerateHeightMap();

	void AddPlate(XMINT4 prop);
};

