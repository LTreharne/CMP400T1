#pragma once
#include <vector>
#include "DXF.h"
#include "Vector2.h"
class Plate
{
public:
	Plate();
	Plate(bool isOceanic);
	std::vector<std::vector<float>> plateHeightMap;
	std::vector<std::vector<bool>> IsPartOfPlate;

	int xOff;
	int yOff;
	float xpos;
	float ypos;
	int width;
	int height;
	float weight;
	float density;


	float crustAge; // will be changed to array later
	
	bool oceanic;
	XMFLOAT2 velocity;
	XMFLOAT2 velocityChanges;

	void GenerateHeightMap();
	void Update();
	void UpdateProperties(XMINT4 p);//mostly vestigial
	void SetIsPartofPlateMap(std::vector<std::vector<bool>> map);
	void OOB();
	void CalcualteWeight();
	void UpdateVelocity();
	bool TryAssignNewCrust(Vector2);
};

