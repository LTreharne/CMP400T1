#include "Seed.h"

void Seed::Init(int width, int height)
{
	left = width;
	top = height;
	bottom = 0;
	right = 0;

	IsPartofPlateMap.resize(width, std::vector<bool>(height, false));

	pos.x = rand() % width;
	pos.y = rand() % height;
}

float Seed::distanceToPoint(int x, int y)
{
	float xdiff = pos.x - x;
	float ydiff = pos.y - y;

	return sqrt(xdiff * xdiff + ydiff * ydiff);
}

void Seed::AddToPlate(int x, int y)
{
	IsPartofPlateMap[x][y] = true;

	left = min(left, x);
	right = max(right, x);

	top = min(top, y);
	bottom = max(bottom, y);
}

std::vector<std::vector<bool>> Seed::getPartofPlateMap()
{
	std::vector<std::vector<bool>> map;

	map.resize((right - left), std::vector<bool>(bottom-top));

	for (int i = 0; i < right-left; i++)
	{
		for (int j = 0; j < bottom-top; j++)
		{
			map[i][j] = IsPartofPlateMap[i + left][j + top];
		}
	}
	return map;
}

XMINT4 Seed::getPlateProperties()
{
	return XMINT4(right-left, bottom-top, left, top);
}
