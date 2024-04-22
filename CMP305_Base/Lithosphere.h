#pragma once
#include <vector>
#include "Plate.h"
#define NOMINMAX
#include <algorithm>
#include <windef.h>
#include "Constants.h"
#include "HotSpot.h"
#include "Seed.h"
#include "Vector2.h"
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
	int PositiveModulus(int i, int n);
	bool AABBCollisionCheck(Plate p1, Plate p2);

	//collisions
	void CollisionCheck(int,int);
	std::vector<Vector2> ElasticCollision(Vector2 v1, Vector2 v2, float m1, float m2,Vector2 x1,Vector2 x2);
	bool ShouldSwapIndicies(int p1, int p2);
};

