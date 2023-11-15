#pragma once
#include <vector>
#include "DXF.h"
class Plate
{
public:
	Plate();
	std::vector<std::vector<float>> plateHeightMap;
	int xOff;
	int yOff;
	int width;
	int height;
	

	void GenerateHeightMap();

	void UpdateProperties(XMINT4 p);
};

