#pragma once
#ifndef VECTOR2F_H
#define VECTOR2F_H

#include <math.h>

//Vector2f
class Vector2f
{
public:
	static const Vector2f Add(const Vector2f& v1, const Vector2f& v2) { return Vector2f(v1.x + v2.x, v1.y + v2.y); }
	static const Vector2f Subtract(const Vector2f& v1, const Vector2f& v2) { return Vector2f(v1.x - v2.x, v1.y - v2.y); }
	static const Vector2f Multiply(const Vector2f& v1, const Vector2f& v2) { return Vector2f(v1.x * v2.x, v1.y * v2.y); }
	static const Vector2f Divide(const Vector2f& v1, const Vector2f& v2) { return Vector2f(v1.x / v2.x, v1.y / v2.y); }
	
	static const Vector2f Distance(const Vector2f& v1, const Vector2f& v2) { return Vector2f(v2.x - v1.x, v2.y - v1.y); }
	static const Vector2f DirectionTo(const Vector2f& v1, const Vector2f& v2) { return Vector2f(v2.x - v1.x, v2.y - v1.y).Normalised(); }

	static const float Magnitude(const Vector2f& v) { return sqrtf((v.x * v.x) + (v.y * v.y)); }
	static const Vector2f Normalize(Vector2f& v) { return Vector2f(v.x / v.Magnitude(), v.y / v.Magnitude()); }
	static const float Dot(const Vector2f& v1, const Vector2f& v2) { return (v1.x * v2.x) + (v1.y * v2.y); }
	static const float Angle(Vector2f& v1, Vector2f& v2) { return acosf(v1.Dot(v2) / (v1.Magnitude() * v2.Magnitude())); }
	static const Vector2f Lerp(const Vector2f& v1, const Vector2f& v2, float t) { return Vector2f(v1.x + (v2.x - v1.x) * t, v1.y + (v2.y - v1.y) * t); }
	
	static const Vector2f One() { return Vector2f(1.0f, 1.0f); }
	static const Vector2f Up() { return Vector2f(0.0f, 1.0f); }
	static const Vector2f Down() { return Vector2f(0.0f, -1.0f); }
	static const Vector2f Left() { return Vector2f(-1.0f, 0.0f); }
	static const Vector2f Right() { return Vector2f(1.0f, 0.0f); }
	
	Vector2f() : x( 0.0f ), y( 0.0f ) {}
	Vector2f( const Vector2f& vec ) : x( vec.x ), y( vec.y ) {}
	Vector2f( const float x, const float y ) : x( x ), y( y ) {}
	Vector2f( const float fAngle) : x(cos(fAngle)), y(sin(fAngle)) {}
	~Vector2f() { ; }

	Vector2f Add(const Vector2f v) { return Vector2f(x + v.x, y + v.y); }
	Vector2f Subtract(const Vector2f v) { return Vector2f(x - v.x, y - v.y); }
	Vector2f Multiply(const float s) { return Vector2f(x * s, y * s); }
	Vector2f Divide(const float s) { return Vector2f(x / s, y / s); }
	
	float Distance(const Vector2f v) { return sqrt(pow(v.x - x, 2) + pow(v.y - y, 2) * 1.0f); }
	Vector2f DirectionTo(const Vector2f v) { return Vector2f(v.x - x, v.y - y).Normalised(); }

	float Magnitude() { return sqrt((x * x) + (y * y)); }
	float Length() { return sqrt(x * x + y * y); }
	float LengthSquared() { return x * x + y * y; }
	float Angle(Vector2f& other) { return acosf(this->Dot(other) / (this->Magnitude() * other.Magnitude())); }
	float Dot(const Vector2f& other) { return (x * other.x) + (y * other.y); }
	Vector2f Normalised() { return this->Divide(this->Magnitude()); }
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

	Vector2f operator + ( const Vector2f& rhs ) { return Vector2f( x + rhs.x, y + rhs.y ); }
	Vector2f operator + ( const Vector2f& rhs ) const { return Vector2f( x + rhs.x, y + rhs.y ); }
	Vector2f operator - ( const Vector2f& rhs ) { return Vector2f( x - rhs.x, y - rhs.y ); }
	Vector2f operator - ( const Vector2f& rhs ) const { return Vector2f( x - rhs.x, y - rhs.y ); }
	Vector2f operator * ( const float& rhs ) { return Vector2f( x * rhs, y * rhs ); }
	Vector2f operator * ( const float rhs ) const { return Vector2f( x * rhs, y * rhs ); }
	Vector2f operator / ( const float& rhs ) { return Vector2f( x / rhs, y / rhs ); }
	Vector2f operator / ( const float rhs ) const { return Vector2f( x / rhs, y / rhs ); }
	
	void operator += ( const Vector2f& vec ) { x += vec.x; y += vec.y; }
	void operator -= ( const Vector2f& vec ) { x -= vec.x; y -= vec.y; }
	void operator *= ( const Vector2f& vec ) { x *= vec.x; y *= vec.y; }
	void operator /= ( const Vector2f& vec ) { x /= vec.x; y /= vec.y; }

	float operator[]( unsigned i ) const { if ( i == 0 ) return x; if ( i == 1 ) return y; }
	float& operator[]( unsigned i ) { if ( i == 0 ) return x; if ( i == 1 ) return y; }
	float& operator [] ( const std::size_t& i ) { return *( ( float* )this + i ); }
	
	float x;
	float y;
	
private:
	
};

#endif // VECTOR2F_H