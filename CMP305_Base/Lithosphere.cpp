#include "Lithosphere.h"

Lithosphere::Lithosphere()
{
	width = 128;
	height = 128;
	lithoHeightMap.resize(width);

	for (int i = 0; i < width;++i) {
		lithoHeightMap[i].resize(height);
	}

	Plate p = Plate(true);
	plates.push_back(p);

	GenerateHeightMap();
}

void Lithosphere::GenerateHeightMap()
{
	//reset heightmap 
	lithoHeightMap = std::vector<std::vector<float>>(width, std::vector<float>(height, 0));
	for (int i = 0; i < plates.size(); i++)
	{
		int pheight = plates[i].height;
		int pwidth = plates[i].width;
		int pxoff = plates[i].xOff;
		int pyoff = plates[i].yOff;

		for (int j = 0; j < pwidth; j++)
		{
			for (int k = 0; k < pheight; k++)
			{
				if (j+pxoff<width&&k+pyoff<height) {
					lithoHeightMap[j + pxoff][k+pyoff]+=plates[i].plateHeightMap[j][k];
				}
			}
		}
	}
}

void Lithosphere::AddPlate(XMINT4 prop, bool isOceanic)
{
	Plate p = Plate(isOceanic);
	p.UpdateProperties(prop);
	plates.push_back(p);
	GenerateHeightMap();
}

void Lithosphere::Itterate()
{
	for (int i = 0; i < plates.size(); ++i){
		plates[i].Update();
	}
	CollisionCheck();	
	GenerateHeightMap();
}


void Lithosphere::CollisionCheck()
{
	float top;
	float bottom;
	float left;
	float right;

	Plate p1 = plates[0];
	Plate p2 = plates[1];

	/*
	AABB collisions
	top left of plate is xoff, yoff
	3 other points based off xoff, yoff, width and height
	*/

	for (int i = 0; i < p2.width; ++i)
	{
		for (int j = 0; j < p2.height; ++j) {
			float d = std::abs(p1.xOff + p1.width - (i + p2.xOff))/10.f;//distance to subduction point (CHANGE LATER TO ARBRITRATY AXIS)
			float distUplift = -7*d*d*d + d*d + d + 0.8;
			plates[1].plateHeightMap[i][j] += baseUplift*max(distUplift,0);
		}
	}

	//check if colliding
	bool colliding = false;
	//if overlap on a vertical and a horizontal axis then collision
	if ((p1.xOff+p1.width>p2.xOff&&p1.xOff<p2.xOff+p2.width)&&(p1.yOff + p1.height > p2.yOff && p1.yOff < p2.yOff + p2.height)) {
			for (int i = 0; i < p1.width; ++i)
			{
				for (int j = 0; j < p1.height; ++j) {
					//plates[0].plateHeightMap[i][j] += 10.f;
				}
			}
			//COLLISION DECECTION 2 plates expand to loop overall plates 
			left = max(p1.xOff, p2.xOff);
			right = min(p1.xOff+p1.width, p2.xOff+p2.width);

			top = max(p1.yOff, p2.yOff);
			bottom = min(p1.yOff + p1.height, p2.yOff + p2.height);
	}
	
}
