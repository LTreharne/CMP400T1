#include "Lithosphere.h"
#include <cmath>
Lithosphere::Lithosphere()
{
	width = lithoWidth;
	height = lithoHeight;
	itterationCount = 0;
	lithoHeightMap.resize(width);

	for (int i = 0; i < width;++i) {
		lithoHeightMap[i].resize(height);
	}	

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
				
					lithoHeightMap[(j + pxoff) % lithoWidth][(k+pyoff)%lithoHeight]+=plates[i].plateHeightMap[j][k] * plates[i].IsPartOfPlate[j][k];
				
			}
		}
	}
}

void Lithosphere::GeneratePlates(int plateCount)
{

	//resizePlates
	plates.clear();
	plates.resize(plateCount);
	
	//Generate Seeds
	std::vector<Seed> seeds(plateCount);
	for (int i = 0; i < seeds.size(); i++)
	{
		seeds[i].Init(width, height);
	}
	
	//CHECK every position for closest seed
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			int minDistanceIndex = 0;
			float minDistance = width * height;

			for (int k = 0; k < seeds.size(); k++)
			{
				if (seeds[k].distanceToPoint(i,j)<minDistance) {
					minDistance = seeds[k].distanceToPoint(i, j);
					minDistanceIndex = k;
				}
			}

			seeds[minDistanceIndex].AddToPlate(i, j);
		}
	}

	for (int i = 0; i < plates.size(); ++i) {
		plates[i].oceanic = (bool)(rand() % 2);
		plates[i].UpdateProperties(seeds[i].getPlateProperties());
		plates[i].SetIsPartofPlateMap(seeds[i].getPartofPlateMap());
		plates[i].CalcualteWeight();
	}

	GenerateHeightMap();
}

void Lithosphere::AddPlate(XMINT4 prop, bool isOceanic)
{
	Plate p = Plate(isOceanic);
	std::vector<std::vector<bool>> m(prop.x, std::vector<bool>(prop.y,true));
	p.UpdateProperties(prop);
	p.SetIsPartofPlateMap(m);
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
	//move plates 
	for (int i = 0; i < plates.size(); ++i){
		plates[i].Update();
	}


	////check for collisions between every generated plate
	for (int i = 0; i < plates.size();++i) {
		for (int j = i+1; j < plates.size(); j++)
		{
			CollisionCheck(i, j);
		}
	}

	//recalculate plate weights
	for (int i = 0; i < plates.size(); ++i) {
		plates[i].CalcualteWeight();
	}

	//HOTSPOTS
	CalcHotSpotUplift();

	//handle items that dont occour every itteration
	itterationCount++;
	if (itterationCount>9) {
		//Assign blank spaces
		AssignBlankSpace();	
		
		//Errosion
		itterationCount = 0;
	}

	//O(n)
	GenerateHeightMap();
}

void Lithosphere::AssignBlankSpace()
{
	std::vector<Vector2> candidates;
	
	int totalEmptySpace = 0;
	do {

		candidates = FindCandidates(totalEmptySpace);
		for (int i = 0; i < candidates.size(); ++i) {
			for (int j = 0; j < plates.size(); ++j) {

				int up = max(0, candidates[i].y - 1);
				int down = min(candidates[i].y + 1, height - 1);

				int left = max(0, candidates[i].x - 1);
				int right = min(width - 1, candidates[i].x + 1);

				//cehck connecting cells for overlap with plate
				if (AABBCollisionCheck(XMFLOAT2(candidates[i].x, up), XMFLOAT2(1, down - up + 1), XMFLOAT2(plates[j].xOff, plates[j].yOff), XMFLOAT2(plates[j].width, plates[j].height)) ||
					AABBCollisionCheck(XMFLOAT2(left, candidates[i].y), XMFLOAT2(right - left + 1, 1), XMFLOAT2(plates[j].xOff, plates[j].yOff), XMFLOAT2(plates[j].width, plates[j].height))) {
					//if overlap exists checks plates exact shape to see if its neighbouring and then assigns it to that plate of so
					if ((plates[j].TryAssignNewCrust(candidates[i]))) {

						break;
					}
				}
			}
		}
		GenerateHeightMap();
	} while (totalEmptySpace > 0);//keep filling spots till no empty space remains
}

std::vector<Vector2> Lithosphere::FindCandidates(int &space)//finds all spots where heightmap is empty but neighbouring cell is full (neightbous must share edge not just corner aka manhattan)
{
	space = 0;
	std::vector<Vector2> c;
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			if (lithoHeightMap[i][j] == 0) {
				space++;
				if (lithoHeightMap[min(i + 1, width - 1)][j] != 0 || lithoHeightMap[max(i - 1, 0)][j] != 0 || lithoHeightMap[i][max(j - 1, 0)] != 0 || lithoHeightMap[i][min(j + 1, height - 1)] != 0) {//check cardianl neighbours and ensure within bounds				
					c.push_back(Vector2(i,j));
				}
			}
		}
	}
	return c;
}

void Lithosphere::CalcHotSpotUplift()//adjusts plate heightmaps based off hotspots
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

int Lithosphere::PositiveModulus(int i, int n)//will return a positive number for modulus of a negative (not default in c++)
{
	return (i % n + n) % n;
}

bool Lithosphere::AABBCollisionCheck(Plate p1, Plate p2)//alterd AABB formula that allows for plates to wrap around and still detect collisions
{
	int dx = PositiveModulus(p2.xOff - p1.xOff, lithoWidth);
	int dy = PositiveModulus(p2.yOff - p1.yOff, lithoHeight);
	return((dx<p1.width||dx+p2.width>lithoWidth)&&(dy<p1.height||dy+p2.height>lithoHeight));
}

bool Lithosphere::AABBCollisionCheck(XMFLOAT2 position, XMFLOAT2 dimentions, XMFLOAT2 targetPosition, XMFLOAT2 targetDimentions)//overload
{
	int dx = PositiveModulus(targetPosition.x - position.x, lithoWidth);
	int dy = PositiveModulus(targetPosition.y - position.y, lithoHeight);
	return((dx<dimentions.x || dx + targetDimentions.x>lithoWidth) && (dy<dimentions.y || dy + targetDimentions.y>lithoHeight));
}


void Lithosphere::CollisionCheck(int index1, int index2)
{
	int top;
	int bottom;
	int left;
	int right;


	if (ShouldSwapIndicies(index1, index2))
		std::swap(index1, index2);
	Plate p1 = plates[index1];
	Plate p2 = plates[index2];



	//if overlap on a vertical and a horizontal axis then collision
	if (AABBCollisionCheck(p1,p2))

	{
			int XCatchScenario4P1 = 0;
			int XcatchScenario4P2 = 0;
			int YCatchScenario4P1 = 0;
			int YcatchScenario4P2 = 0;

			//catch scenario 4
			if ((p1.xOff + p1.width > lithoWidth) && (p2.xOff + p2.width < p1.xOff))
				XCatchScenario4P1 = lithoWidth;
			else if ((p2.xOff + p2.width > lithoWidth) && (p1.xOff + p1.width < p2.xOff))
				XcatchScenario4P2 = lithoWidth;	

			if ((p1.yOff + p1.height > lithoHeight) && (p2.yOff + p2.height < p1.yOff))
				YCatchScenario4P1 = lithoHeight;
			else if ((p2.yOff + p2.height > lithoHeight) && (p1.yOff + p1.height < p2.yOff))
				YcatchScenario4P2 = lithoHeight;
			

			//COLLISION DECECTION 2 plates expand to loop overall plates 
			left = max(p1.xOff-XCatchScenario4P1, p2.xOff-XcatchScenario4P2);
			right = min(p1.xOff+p1.width-XCatchScenario4P1, p2.xOff+p2.width-XcatchScenario4P2);

			top = max(p1.yOff-YCatchScenario4P1, p2.yOff- YcatchScenario4P2);
			bottom = min(p1.yOff + p1.height- YCatchScenario4P1, p2.yOff + p2.height-YcatchScenario4P2);

			

			
			//CHECK THE 4 edges to for enhanced collision detection
			bool isColliding = false;

			for (int i = 0; i < right-left; ++i)
			{
				int x1 = PositiveModulus(left-p1.xOff,lithoWidth) + i;
				int x2 = PositiveModulus(left-p2.xOff,lithoWidth) + i;
				int y1 = PositiveModulus(top - p1.yOff, lithoHeight);
				int y2 = PositiveModulus(top - p2.yOff, lithoHeight);
				int x = 1;
				isColliding |= (p1.IsPartOfPlate[x1][y1] &p2.IsPartOfPlate[x2][y2]);
	
			}

			

			for (int i = 0; i < right - left; ++i)
			{
				int x1 = PositiveModulus(left - p1.xOff, lithoWidth) + i;
				int x2 = PositiveModulus(left - p2.xOff, lithoWidth) + i;
				int y1 = PositiveModulus(bottom - p1.yOff, lithoHeight)-1;
				int y2 = PositiveModulus(bottom - p2.yOff, lithoHeight)-1;
				int x = 1;
				isColliding |= (p1.IsPartOfPlate[x1][y1] & p2.IsPartOfPlate[x2][y2]);

			}
			
			for (int i = 0; i < bottom - top-1; ++i)
			{
				int x1 = PositiveModulus(right - p1.xOff, lithoWidth)-1;
				int x2 = PositiveModulus(right - p2.xOff, lithoWidth)-1;
				int y1 = PositiveModulus(top - p1.yOff, lithoHeight) + i;
				int y2 = PositiveModulus(top - p2.yOff, lithoHeight) + i;
				isColliding |= (p1.IsPartOfPlate[x1][y1] & p2.IsPartOfPlate[x2][y2]);
				
			}

			for (int i = 0; i < bottom - top - 1; ++i)
			{
				int x1 = PositiveModulus(left - p1.xOff, lithoWidth);
				int x2 = PositiveModulus(left - p2.xOff, lithoWidth);
				int y1 = PositiveModulus(top - p1.yOff, lithoHeight) + i;
				int y2 = PositiveModulus(top - p2.yOff, lithoHeight) + i;
				isColliding |= (p1.IsPartOfPlate[x1][y1] & p2.IsPartOfPlate[x2][y2]);

			}
			
			
			
			
			
			
			
			if (isColliding) {

				//UPDATE velocityies
				Vector2 v1, v2, X1, X2;
				v1 = Vector2(p1.velocity.x, p1.velocity.y);
				v2 = Vector2(p2.velocity.x, p2.velocity.y);
				X1 = Vector2(p1.xOff+p1.width/2, p1.yOff+p1.height/2);
				X2 = Vector2(p2.xOff + p2.width / 2, p2.yOff + p2.height / 2);
				std::vector<Vector2> changes = ElasticCollision(v1, v2, p1.weight, p2.weight, X1, X2);

				plates[index1].velocity.x += changes[0].x;
				plates[index1].velocity.y += changes[0].y;
				plates[index2].velocity.x += changes[1].x;
				plates[index2].velocity.y += changes[1].y;

				 p1 = plates[index1];
				 p2 = plates[index2];

				//CHECK IF ITS ABOVE OR BELOW OTHER PLATE

		
				for (int i = 0; i < p2.width; ++i)
				{
					for (int j = 0; j < p2.height; ++j) {

						float xCoord = (p2.xOff + i)%lithoWidth;
						float yCoord = (p2.yOff + j)%lithoHeight;
					
						float theta = (atan2(p1.velocity.x, p1.velocity.y) + atan2(p2.velocity.x, p2.velocity.y))/2.f;

						float d = std::abs(cos(theta) * (((top+bottom)/2.f) - yCoord) - sin(theta) * (((right+left)/ 2.f) - xCoord));
						if (d == 0)
							d = 0.271f;
						else
							d /= 10;

						float distUplift = ( - 7 * d * d * d) + (d * d) + d + 0.8f;

						XMFLOAT2 relativeSpeed = XMFLOAT2(plates[index1].velocity.x - plates[index2].velocity.x, plates[index1].velocity.y - plates[index2].velocity.y);
						float velocityFactor = sqrt(relativeSpeed.x * relativeSpeed.x + relativeSpeed.y * relativeSpeed.y) / plateMaximumSpeed;

						plates[index2].plateHeightMap[i][j] += baseUplift * max(distUplift, 0) * velocityFactor;
					}
				}
			}



	}






}

std::vector<Vector2> Lithosphere::ElasticCollision(Vector2 v1, Vector2 v2, float m1, float m2, Vector2 x1, Vector2 x2)//update velocities of plates based off weights according to newtonian laws
{
	std::vector<Vector2> changes;
	Vector2 v1After = (x1 - x2) * (((-2 * m2) / (m1 + m2)) * (((v1 - v2).Dot(x1 - x2))/ pow(((x1 - x2).Mag()), 2)));
	Vector2 v2After = (x2 - x1) * (((-2 * m1) / (m1 + m2)) * (((v2 - v1).Dot(x2 - x1))/ pow(((x2 - x1).Mag()), 2)));
	changes.push_back(v1After);
	changes.push_back(v2After);
	return changes;
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

