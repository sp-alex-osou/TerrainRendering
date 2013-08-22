#pragma once

#include <d3dx9.h>

#include "Vector3.h"
#include "Vector2.h"

struct Vector4 : public D3DXVECTOR4
{
public:
	Vector4();
	Vector4(float x, float y, float z, float w = 1.0f);
	Vector4(const Vector3& v, float w = 1.0f);
	Vector4(const Vector2& v);
	Vector4(const float* f);
	Vector4(const D3DCOLORVALUE& color);

	operator Vector3() const;
	operator D3DCOLORVALUE() const;

	Vector4 operator*(const Vector4& v) const;

	void operator*=(const Vector4& v);

	static const Vector4 one;
	static const Vector4 zero;
};