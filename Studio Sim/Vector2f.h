#pragma once
#ifndef VECTOR2F_H
#define VECTOR2F_H

#include <math.h>

//Vector2f
class Vector2f
{
public:
	Vector2f();
	Vector2f(float x, float y);
	~Vector2f();

	float x;
	float y;

	Vector2f Add(Vector2f v) { return Vector2f(x + v.x, y + v.y); }
	Vector2f Subtract(Vector2f v) { return Vector2f(x - v.x, y - v.y); }
	Vector2f Multiply(const float s) { return Vector2f(x * s, y * s); }
	Vector2f Divide(const float s) { return Vector2f(x / s, y / s); }
	
	float Distance(Vector2f v) { return sqrt(pow(v.x - x, 2) + pow(v.y - y, 2) * 1.0f); }
	float Magnitude() { return sqrt((x * x) + (y * y)); }
	float Length() { return sqrt(x * x + y * y); }
	float LengthSquared() { return x * x + y * y; }
	float Dot(const Vector2f& other) { return (x * other.x) + (y * other.y); }
	Vector2f Normalised() { return this->Divide(Magnitude()); }
	Vector2f LerpTo(const Vector2f v, const float s) 
	{ 
		return Vector2f(
			x + (v.x - x) * s,
			y + (v.y - y) * s); 
	}
	Vector2f Rotate(const float angle)
	{
		return Vector2f(
			x * cos(angle) - y * sin(angle),
			x * sin(angle) + y * cos(angle));
	}
	
private:
	
};

#endif // VECTOR2F_H