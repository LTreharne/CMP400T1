#pragma once
#include <vector>
#include "DXF.h"
class Plate
{
public:
	Plate();
	Plate(bool isOceanic);
	std::vector<std::vector<float>> plateHeightMap;
	std::vector<std::vector<bool>> IsPartOfPlate;

	float xOff;
	float yOff;
	int width;
	int height;


	float crustAge; // will be changed to array later
	
	bool oceanic;
	XMFLOAT2 velocity;
	

	void GenerateHeightMap();
	void Update();
	void UpdateProperties(XMINT4 p);
	void SetIsPartofPlateMap(std::vector<std::vector<bool>> map);
};

