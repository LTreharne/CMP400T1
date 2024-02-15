#pragma once
#include "DXF.h"
class HotSpot
{
public:
	HotSpot() = default;
	HotSpot(XMFLOAT4 prop, XMFLOAT2 vel);

	float xPos;
	float yPos;
	float radi;
	float upliftAmount;
	XMFLOAT2 velocity;

	void Update();

};

