#pragma once
#include <vector>
#include "DXF.h"
class Seed
{
public:
	void Init(int width, int height);
	float distanceToPoint(int x, int y);
	void AddToPlate(int x, int y);
	std::vector<std::vector<bool>> getPartofPlateMap();
	XMINT4 getPlateProperties();
private:
	std::vector<std::vector<bool>> IsPartofPlateMap;
	XMINT2 pos;
	int left, right, top, bottom;

};

