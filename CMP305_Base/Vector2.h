#pragma once
class Vector2
{
public:
	float x, y;
	Vector2(float xin, float yin) { x = xin; y = yin; }
	Vector2() = default;
	float Dot(Vector2 v);
	float Mag();

	void operator+=(Vector2 v);
	void operator-=(Vector2 v);

	void operator+=(float v);
	void operator-=(float v);

	void operator*=(float s);
	void operator/=(float s);	

	Vector2 operator+(Vector2 v) { return Vector2(x + v.x, y + v.y); };
	Vector2 operator-(Vector2 v) { return Vector2(x - v.x, y - v.y); };
	Vector2 operator*(float s) { return Vector2(x * s, y *s); };
	Vector2 operator/(float s) { return Vector2(x * s, y *s); };

};

