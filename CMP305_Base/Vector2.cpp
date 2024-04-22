#include "Vector2.h"
#include <cmath>
float Vector2::Dot(Vector2 v)
{
	return x * v.x + y * v.y;
}

float Vector2::Mag()
{
	return sqrt(x * x + y * y);
}

void Vector2::operator+=(Vector2 v)
{
	x += v.x;
	y += v.y;
}

void Vector2::operator-=(Vector2 v)
{
	x -= v.x;
	y -= v.y;
}

void Vector2::operator+=(float v)
{
	x += v;
	y += v;
}

void Vector2::operator-=(float v)
{
	x -= v;
	y -= v;
}


void Vector2::operator*=(float s)
{
	x *= s;
	y *= s;
}

void Vector2::operator/=(float s)
{

	x /= s;
	y /= s;
}
