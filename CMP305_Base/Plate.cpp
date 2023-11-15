
#include "Plate.h"

Plate::Plate()
{
	width = 10;
	height = 10;
	xOff = 10;
	yOff = 10;
	//plateHeightMap.resize(width);

	//for (int i = 0; i < width; ++i) {
	//	plateHeightMap[i].resize(height);
	//}

	GenerateHeightMap();
}

void Plate::GenerateHeightMap()
{

	plateHeightMap.resize(width);

	for (int i = 0; i < width; ++i) {
		plateHeightMap[i].resize(height);
	}


	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j) {
			plateHeightMap[i][j] = 1.f;
		}
	}
}

void Plate::UpdateProperties(XMINT4 p)
{
	width = p.x;
	height = p.y;
	xOff = p.z;
	yOff = p.w;

	GenerateHeightMap();
}
