#pragma once

#include <d3dx9math.h>

#include <iostream>

#include "Vector2.h"

struct Matrix;
struct Vector4;

struct Vector3 : public D3DXVECTOR3
{
	friend std::ostream& operator<<(std::ostream&, const Vector3&);
	friend std::istream& operator>>(std::istream&, const Vector3&);

	Vector3();
	Vector3(float f);
	Vector3(float x, float y, float z);
	Vector3(const D3DXVECTOR3& v);
	Vector3(const Vector2& v);
	Vector3(const D3DCOLORVALUE& v);
	Vector3(const D3DVECTOR v);

	operator D3DCOLORVALUE() const;

	void normalize();
	float length() const;
	float lengthSquared() const;

	static Vector3 lerp(const Vector3& v1, const Vector3& v2, float s);
	static Vector3 normalize(const Vector3& v);
	static Vector3 transform(const Vector3& v, const Matrix& m);
	static Vector3 cross(const Vector3& v1, const Vector3& v2);
	static float dot(const Vector3& v1, const Vector3& v2);

	Vector3 operator-() const;

	Vector3 operator-(const Vector3& v) const;
	Vector3 operator*(const Vector3& v) const;

	bool operator==(const Vector3& v) const;
	bool operator!=(const Vector3& v) const;

	void operator*=(const Vector3& v);

	static const Vector3 zero;
	static const Vector3 one;

	static const Vector3 up;
	static const Vector3 down;
	static const Vector3 forward;
	static const Vector3 back;
	static const Vector3 right;
	static const Vector3 left;

private:
	static Vector3 v;
};


std::ostream& operator<<(std::ostream& os, const Vector3& v);
std::istream& operator>>(std::istream& is, Vector3& v);