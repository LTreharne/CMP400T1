#include "Lithosphere.h"
#include <cmath>
Lithosphere::Lithosphere()
{
	width = 128;
	height = 128;
	lithoHeightMap.resize(width);

	for (int i = 0; i < width;++i) {
		lithoHeightMap[i].resize(height);
	}

	/*Plate p = Plate(true);
	plates.push_back(p);*/

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
					lithoHeightMap[j + pxoff][k+pyoff]+=plates[i].plateHeightMap[j][k] * plates[i].IsPartOfPlate[j][k];
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

void Lithosphere::AddHotSpot(XMFLOAT4 prop, XMFLOAT2 vel)
{
	HotSpot h = HotSpot(prop, vel);
	hotSpots.push_back(h);
}

void Lithosphere::Itterate()
{
	//move plates O(n)
	for (int i = 0; i < plates.size(); ++i){
		plates[i].Update();
	}


	//check for collisions between every generated plate
	for (int i = 0; i < plates.size();++i) {
		for (int j = i+1; j < plates.size(); j++)
		{
			CollisionCheck(i, j);
		}
	}

	//HOTSPOTS

	CalcHotSpotUplift();


	//O(n)
	GenerateHeightMap();
}

void Lithosphere::CalcHotSpotUplift()
{
	for (int i = 0; i < hotSpots.size(); i++)
	{
		for (int j = 0; j < plates.size(); ++j) {
			Plate p = plates[j];
				for (int k = 0; k < p.width; ++k)
				{
					for (int l = 0; l < p.height; ++l)
					{
						float xChange = hotSpots[i].xPos - p.xOff - k;
						float yChange = hotSpots[i].yPos - p.yOff - l;
						float dist = min(hotSpots[i].radi, sqrt(xChange * xChange + yChange * yChange))/ hotSpots[i].radi;
						float uplift = hotSpots[i].upliftAmount * (1-dist);
						plates[j].plateHeightMap[k][l] += uplift;
					}
				}
		}
		hotSpots[i].Update();
	}
}


void Lithosphere::CollisionCheck(int index1, int index2)
{
	float top;
	float bottom;
	float left;
	float right;


	if (ShouldSwapIndicies(index1, index2))
		std::swap(index1, index2);
	Plate p1 = plates[index1];
	Plate p2 = plates[index2];


	//if overlap on a vertical and a horizontal axis then collision
	if ((p1.xOff+p1.width>p2.xOff&&p1.xOff<p2.xOff+p2.width)&&(p1.yOff + p1.height > p2.yOff && p1.yOff < p2.yOff + p2.height)) {
		
		

			//COLLISION DECECTION 2 plates expand to loop overall plates 
			left = max(p1.xOff, p2.xOff);
			right = min(p1.xOff+p1.width, p2.xOff+p2.width);

			top = max(p1.yOff, p2.yOff);
			bottom = min(p1.yOff + p1.height, p2.yOff + p2.height);

			

			
			//CHECK THE 4 edges to for enhanced collision detection
			bool isColliding = false;

			for (int i = 0; i < right-left; ++i)
			{
				isColliding |= (p1.IsPartOfPlate[left - p1.xOff + i][top - p1.yOff] & p2.IsPartOfPlate[left - p2.xOff + i][top - p2.yOff]);
	
			}

			for (int i = 0; i < top - bottom; ++i)
			{
				isColliding |= (p1.IsPartOfPlate[left - p1.xOff][top - p1.yOff+i] & p2.IsPartOfPlate[left - p2.xOff][top - p2.yOff+i]);
			}

			for (int i = 0; i < right - left; ++i)
			{
				isColliding |= (p1.IsPartOfPlate[left - p1.xOff + i][bottom-p1.yOff - 1] & p2.IsPartOfPlate[left - p2.xOff + i][bottom - p2.yOff - 1]);

			}
			for (int i = 0; i < top - bottom; ++i)
			{
				isColliding |= (p1.IsPartOfPlate[right - p1.xOff ][top - p1.yOff + i] & p2.IsPartOfPlate[right - p2.xOff][top - p2.yOff + i]);
			}
			
			
			
			
			
			
			
			
			
			if (isColliding) {


				//FIND LEFTMOST PLATE
				int leftMostIndex, rightmostIndex;
				if (p1.xOff > p2.xOff) {
					leftMostIndex = index1;
					rightmostIndex = index2;
				}
				else
				{
					leftMostIndex = index2;
					rightmostIndex = index1;
				}

				//CHECK IF ITS ABOVE OR BELOW OTHER PLATE
				bool isABove = false;
				if (plates[leftMostIndex].yOff < plates[rightmostIndex].yOff) isABove = true;
				//USE CORISPONDING AXIS
				float x1, y1, x2, y2;

				x1 = left;
				x2 = right;
				if (isABove) {
					y1 = top;
					y2 = bottom;
				}
				else
				{
					y1 = bottom;
					y2 = top;
				}

				for (int i = 0; i < p2.width; ++i)
				{
					for (int j = 0; j < p2.height; ++j) {

						float xCoord = p2.xOff + i;
						float yCoord = p2.yOff + j;


						float yChange = y2 - y1;
						float xChange = x2 - x1;
						float pointXchange = x1 - xCoord;
						float pointYchange = y1 - yCoord;
						float d = std::abs(xChange * pointYchange - yChange * pointXchange) / sqrt(xChange * xChange + yChange * yChange);
						if (d == 0)
							d = 1;
						else
							d /= 10;


						float distUplift = -7 * d * d * d + d * d + d + 0.8;

						XMFLOAT2 relativeSpeed = XMFLOAT2(plates[index1].velocity.x - plates[index2].velocity.x, plates[index1].velocity.y - plates[index2].velocity.y);
						float velocityFactor = sqrt(relativeSpeed.x * relativeSpeed.x + relativeSpeed.y * relativeSpeed.y) / plateMaximumSpeed;

						plates[index2].plateHeightMap[i][j] += baseUplift * max(distUplift, 0) * velocityFactor;
					}
				}
			}



	}






}

bool Lithosphere::ShouldSwapIndicies(int p1, int p2)
{
	//checks if p1 is subducting under p2 (ASSUMED BY DEFAULT FALSE = NO CHANGE)

	if (plates[p1].oceanic == plates[p2].oceanic) {//both the same type of plate AGE based (for now)
		if (plates[p1].crustAge> plates[p2].crustAge) {//older plate subducts
			return false;
		}
		return true;
	}
	else if (plates[p1].oceanic) {//if p1 is oceanic
		return false;
	}
	return true;
}

