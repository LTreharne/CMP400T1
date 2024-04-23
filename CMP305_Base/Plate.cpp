
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
	ypos = 0;
	xpos = 0;
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
	xpos += velocity.x;
	ypos += velocity.y;

	xOff = xpos;
	yOff = ypos;
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
	xpos = p.z;

	yOff = p.w;
	ypos = p.w;
	OOB();
}

void Plate::SetIsPartofPlateMap(std::vector<std::vector<bool>> map)
{
	IsPartOfPlate = map;
}

void Plate::OOB()//ensures plates dont go out of bounds
{
		if (xOff < 0) {
			xpos += lithoWidth;
			xOff = xpos;
		}

		if (xOff > lithoWidth) {
			xpos -= lithoWidth;
			xOff = xpos;
		}

	
		if (yOff < 0) {
			ypos += lithoHeight;
			yOff = ypos;
		}

		if (yOff > lithoHeight) {
			ypos -= lithoHeight;
			yOff = ypos;
		}
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

bool Plate::TryAssignNewCrust(Vector2 position)
{
	int Xlocal = position.x - xOff;
	int Ylocal = position.y - yOff;

	if (Xlocal <-1) {
		Xlocal += lithoWidth;
	}
	if (Ylocal <-1) {
		Ylocal += lithoHeight;
	}
	bool isNeighbouring = false;

	//ensureInbouns in case of wrapping
	
	//check right
	if (Xlocal +1<width&& Ylocal >=0 && Ylocal <height) {//check inbounds
		isNeighbouring |= IsPartOfPlate[Xlocal + 1][Ylocal];
	}

	//check left
	if (Xlocal - 1 >= 0 && Ylocal >= 0 && Ylocal < height ) {//check inbounds
		isNeighbouring |= IsPartOfPlate[Xlocal -1][Ylocal];
	}

	//check Down
	if (Ylocal + 1 < height && Xlocal >= 0 && Xlocal < width ) {//check inbounds
		isNeighbouring |= IsPartOfPlate[Xlocal][Ylocal+1];
	}

	//Check Up
	if (Ylocal-1>=0 && Xlocal >= 0 && Xlocal < width) {//check inbounds
		isNeighbouring |= IsPartOfPlate[Xlocal][Ylocal-1];
	}

	if (isNeighbouring) {

		float remp;
		if (oceanic)
			remp = 0.5;
		else
			remp = 1;

		if (Xlocal >= 0 && Xlocal < width && Ylocal >= 0 && Ylocal < height) {//check inbounds no map expansion required
			IsPartOfPlate[Xlocal][Ylocal] = true;
			plateHeightMap[Xlocal][Ylocal] = remp + (rand() % 5) / 40.f;
		}
		else//rezie needed
		{
			if (Ylocal==height) {//expand Down
				for (int i = 0; i < width; ++i) {
					plateHeightMap[i].push_back(remp + (rand() % 5) / 40.f);
					IsPartOfPlate[i].push_back(false);
				}
				IsPartOfPlate[Xlocal][Ylocal] = true;
				height++;
			}
			if (Xlocal == width) {//expand Down

				IsPartOfPlate.push_back(std::vector<bool>(height, false));
				plateHeightMap.push_back(std::vector<float>(height, 0));

				for (int i = 0; i < height; ++i) {
					plateHeightMap[width][i] = remp + (rand() % 5) / 40.f;
				}

				IsPartOfPlate[Xlocal][Ylocal] = true;
				width++;
			}
			if (Xlocal==-1) {
				IsPartOfPlate.insert(IsPartOfPlate.begin(), std::vector<bool>(height, false));
				plateHeightMap.insert(plateHeightMap.begin(),std::vector<float>(height, 0));

				for (int i = 0; i < height; ++i) {
					plateHeightMap[0][i] = remp + (rand() % 5) / 40.f;
				}

				IsPartOfPlate[0][Ylocal] = true;

				width++;
				xpos--;
				xOff--;
				OOB();
			}
			if (Ylocal == -1) {//expand Down
				for (int i = 0; i < width; ++i) {
					plateHeightMap[i].insert(plateHeightMap[i].begin(), remp + (rand() % 5) / 40.f);
					IsPartOfPlate[i].insert(IsPartOfPlate[i].begin(), false);
				}
				IsPartOfPlate[Xlocal][0] = true;

				height++;
				ypos--;
				yOff--;
				OOB();
			}

		}
	}

	return isNeighbouring;;
}
