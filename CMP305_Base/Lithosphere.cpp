#include "Lithosphere.h"

Lithosphere::Lithosphere()
{
	width = 128;
	height = 128;
	lithoHeightMap.resize(width);

	for (int i = 0; i < width;++i) {
		lithoHeightMap[i].resize(height);
	}

	Plate p;
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

void Lithosphere::AddPlate(XMINT4 prop)
{
	Plate p;
	p.UpdateProperties(prop);
	plates.push_back(p);
	GenerateHeightMap();
}
