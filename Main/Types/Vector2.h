#pragma once

#include <d3dx9math.h>

struct Vector2 : public D3DXVECTOR2
{
	Vector2();
	Vector2(float x, float y);
	Vector2(const D3DXVECTOR2& v);

	static const Vector2 zero;
};