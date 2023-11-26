#pragma once
#include <vector>
#include "DXF.h"
class Plate
{
public:
	Plate(bool isOceanic);
	std::vector<std::vector<float>> plateHeightMap;
	float xOff;
	float yOff;
	int width;
	int height;
	bool oceanic;
	XMFLOAT2 velocity;
	

	void GenerateHeightMap();
	void Update();
	void UpdateProperties(XMINT4 p);
};

