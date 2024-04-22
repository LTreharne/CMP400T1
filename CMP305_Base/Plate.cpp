
#include "Plate.h"
#include "Constants.h"
Plate::Plate(bool isOceanic)
{
	width = 64;
	height = 128;
	xOff = 0;
	yOff = 0;
	oceanic = isOceanic;
	if (oceanic) {
		density = oceanicDesnsity;
	}
	else
	{
		density = continentalDesnsity;
	}
	velocity.x = ((rand() % (int)plateMaximumSpeed * 100) - sqrt(plateMaximumSpeed) * 100) / 100.f;
	velocity.y = ((rand() % (int)plateMaximumSpeed * 100) - sqrt(plateMaximumSpeed) * 100) / 100.f;

	velocityChanges.x = 0;
	velocityChanges.y = 0;

	GenerateHeightMap();
}


Plate::Plate()
{
	width = 64;
	height = 128;
	xOff = 0;
	yOff = 0;
	oceanic = false;
	velocity.x = ((rand() % (int)plateMaximumSpeed * 100) - sqrt(plateMaximumSpeed) * 100) / 100.f;
	velocity.y = ((rand() % (int)plateMaximumSpeed * 100) - sqrt(plateMaximumSpeed) * 100) / 100.f;

	velocityChanges.x = 0;
	velocityChanges.y = 0;
	if (oceanic) {
		density = oceanicDesnsity;
	}
	else
	{
		density = continentalDesnsity;
	}
	GenerateHeightMap();
}

void Plate::GenerateHeightMap()
{

	plateHeightMap.resize(width);
	IsPartOfPlate.resize(width);

	for (int i = 0; i < width; ++i) {
		plateHeightMap[i].resize(height);
		IsPartOfPlate[i].resize(height);
	}


	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j) {

			int remp = rand()%5;
			if (oceanic)
				plateHeightMap[i][j] = 0.5 + remp / 40.f;
			else
				plateHeightMap[i][j] = 1 + remp / 40.f;

		}
	}
}

void Plate::Update()
{
	xOff += velocity.x;
	yOff += velocity.y;
	OOB();
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
	OOB();
}

void Plate::SetIsPartofPlateMap(std::vector<std::vector<bool>> map)
{
	IsPartOfPlate = map;
}

void Plate::OOB()
{
	if (xOff < 0)
		xOff += lithoWidth;

	if (xOff > lithoWidth)
		xOff -= lithoWidth;

	
	if (yOff < 0)
		yOff += lithoHeight;

	if (yOff > lithoHeight)
		yOff -= lithoHeight;
}

void Plate::CalcualteWeight()
{
	weight = 0;
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height;++j) {
			weight += (plateHeightMap[i][j] * IsPartOfPlate[i][j] * density);
		}
	}
}

void Plate::UpdateVelocity()
{
	velocity.x += velocityChanges.x;
	velocity.x = max(velocity.x, -sqrt(plateMaximumSpeed));
	velocity.x = min(velocity.x, sqrt(plateMaximumSpeed));

	velocity.y += velocityChanges.y;
	velocity.y = max(velocity.x, -sqrt(plateMaximumSpeed));
	velocity.y = min(velocity.x, sqrt(plateMaximumSpeed));

	velocityChanges.x = 0;
	velocityChanges.y = 0;
}
