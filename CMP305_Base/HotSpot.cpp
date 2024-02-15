#include "HotSpot.h"

HotSpot::HotSpot(XMFLOAT4 prop, XMFLOAT2 vel)
{
	xPos = prop.x;
	yPos = prop.y;
	radi = prop.z;
	upliftAmount = prop.w;

	velocity = vel;
}

void HotSpot::Update()
{
	xPos += velocity.x;
	yPos += velocity.y;
}


