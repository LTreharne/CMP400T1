
#include "Plate.h"

Plate::Plate(bool isOceanic)
{
	width = 64;
	height = 128;
	xOff = 0;
	yOff = 0;
	oceanic = isOceanic;
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
			int remp = rand() % 5;
			if (oceanic)
				plateHeightMap[i][j] = 0.5+remp/20.f;
			else
				plateHeightMap[i][j] = 1+remp/20.f;
		}
	}
}

void Plate::Update()
{
	/*xOff += velocity.x;
	yOff += velocity.y;*/
}

void Plate::UpdateProperties(XMINT4 p)
{
	if (p.x!=width||p.y!=height) {
		
		width = p.x;
		height = p.y;
		GenerateHeightMap();
	}
	xOff = p.z;
	yOff = p.w;

}
