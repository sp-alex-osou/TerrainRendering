#include "Vector4.h"

const Vector4 Vector4::one = Vector4(1, 1, 1, 1);
const Vector4 Vector4::zero = Vector4(0, 0, 0, 0);

Vector4::Vector4() : D3DXVECTOR4(0, 0, 0, 0) {}
Vector4::Vector4(float x, float y, float z, float w) : D3DXVECTOR4(x, y, z, w) {}
Vector4::Vector4(const Vector3& v, float w) : D3DXVECTOR4(v, w) {}
Vector4::Vector4(const Vector2& v) : D3DXVECTOR4(Vector3(v), 1) {}
Vector4::Vector4(const float* f) : D3DXVECTOR4(f) {}
Vector4::Vector4(const D3DCOLORVALUE& color) : D3DXVECTOR4(color.r, color.g, color.b, color.a) {}

Vector4::operator Vector3() const
{
	return Vector3(x, y, z);
}


Vector4::operator D3DCOLORVALUE() const
{
	D3DCOLORVALUE c;

	c.r = x;
	c.g = y;
	c.b = z;
	c.a = w;

	return c;
}


void Vector4::operator*=(const Vector4& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
}


Vector4 Vector4::operator*(const Vector4& v) const
{
	Vector4 tmp = *this;

	tmp *= v;

	return tmp;
}