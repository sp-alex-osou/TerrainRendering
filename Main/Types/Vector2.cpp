#include "Vector2.h"


Vector2::Vector2(float x, float y) : D3DXVECTOR2(x, y) {}
Vector2::Vector2() : D3DXVECTOR2(0, 0) {}
Vector2::Vector2(const D3DXVECTOR2& v) : D3DXVECTOR2(v) {}

const Vector2 Vector2::zero = Vector2(0, 0);
