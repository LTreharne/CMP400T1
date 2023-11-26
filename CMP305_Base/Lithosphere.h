#pragma once
#include <vector>
#include "Plate.h"
#define NOMINMAX
#include <algorithm>
#include <windef.h>
#include "Constants.h"
class Lithosphere
{
public:
	Lithosphere();

	int width;
	int height;
	std::vector < std::vector<float>> lithoHeightMap;
	std::vector<Plate> plates;

	void GenerateHeightMap();

	void AddPlate(XMINT4 prop, bool);

	void Itterate();

	void CollisionCheck();
};

