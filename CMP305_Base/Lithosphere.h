#pragma once
#include <vector>
#include "Plate.h"
#define NOMINMAX
#include <algorithm>
#include <windef.h>
#include "Constants.h"
#include "HotSpot.h"
#include "Seed.h"
class Lithosphere
{
public:
	Lithosphere();

	int width;
	int height;
	std::vector < std::vector<float>> lithoHeightMap;
	std::vector<Plate> plates;
	std::vector<HotSpot> hotSpots;
	void GenerateHeightMap();
	void GeneratePlates(int plateCount);

	void AddPlate(XMINT4 prop, bool);
	void AddHotSpot(XMFLOAT4 prop, XMFLOAT2 vel);

	//Updating
	void Itterate();

	void CalcHotSpotUplift();


	//collisions
	void CollisionCheck(int,int);
	bool ShouldSwapIndicies(int p1, int p2);
};

