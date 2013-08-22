#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"

#include "..\Helpers\Helpers.h"

const Vector3 Vector3::zero = Vector3(0, 0, 0);
const Vector3 Vector3::one = Vector3(1, 1, 1);

const Vector3 Vector3::right = Vector3(1, 0, 0);
const Vector3 Vector3::up = Vector3(0, 1, 0);
const Vector3 Vector3::forward = Vector3(0, 0, 1);

const Vector3 Vector3::left = -Vector3::right;
const Vector3 Vector3::down = -Vector3::up;
const Vector3 Vector3::back = -Vector3::forward;


Vector3::Vector3() : D3DXVECTOR3(0, 0, 0) {}
Vector3::Vector3(D3DVECTOR v) : D3DXVECTOR3(v.x, v.y, v.z) {}
Vector3::Vector3(float f) : D3DXVECTOR3(f, f, f) {}
Vector3::Vector3(float x, float y, float z) : D3DXVECTOR3(x, y, z) {}
Vector3::Vector3(const D3DXVECTOR3& v) : D3DXVECTOR3(v) {}
Vector3::Vector3(const Vector2& v) : D3DXVECTOR3(v.x, v.y, 1) {}
Vector3::Vector3(const D3DCOLORVALUE& v) : D3DXVECTOR3(v.r, v.g, v.b) {}

Vector3 Vector3::v;


Vector3::operator D3DCOLORVALUE() const
{
	return Vector4(*this);
}


Vector3 Vector3::transform(const Vector3& vector, const Matrix& matrix)
{
	return *D3DXVec3TransformCoord(&v, &vector, &matrix);
}


void Vector3::normalize()
{
	D3DXVec3Normalize(this, this);
}


Vector3 Vector3::normalize(const Vector3& vector)
{
	return *D3DXVec3Normalize(&v, &vector);
}


float Vector3::length() const
{
	return D3DXVec3Length(this);
}


Vector3 Vector3::operator-(const Vector3& v) const
{
	return D3DXVECTOR3::operator-(v);
}


Vector3 Vector3::operator-() const
{
	return D3DXVECTOR3::operator-();
}


Vector3 Vector3::operator*(const Vector3& v) const
{
	Vector3 returnValue;

	returnValue.x = this->x * v.x;
	returnValue.y = this->y * v.y;
	returnValue.z = this->z * v.z;

	return returnValue;
}


void Vector3::operator*=(const Vector3& v)
{
	*this = *this * v;
}


bool Vector3::operator==(const Vector3& v) const
{
	return D3DXVECTOR3::operator==(v) != 0;
}


bool Vector3::operator!=(const Vector3& v) const
{
	return D3DXVECTOR3::operator!=(v) != 0;
}


Vector3 Vector3::cross(const Vector3& v1, const Vector3& v2)
{
	return *D3DXVec3Cross(&v, &v1, &v2);
}


Vector3 Vector3::lerp(const Vector3& v1, const Vector3& v2, float s)
{
	return *D3DXVec3Lerp(&v, &v1, &v2, s);
}


float Vector3::dot(const Vector3& v1, const Vector3& v2)
{
	return D3DXVec3Dot(&v1, &v2);
}


float Vector3::lengthSquared() const
{
	return D3DXVec3LengthSq(this);
}


std::ostream& operator<<(std::ostream& os, const Vector3& v)
{
	os << v.x << "," << v.y << "," << v.z;

	return os;
}


std::istream& operator>>(std::istream& is, Vector3& v)
{
	static std::string tmp[3];

	for (unsigned i = 0; i < 3; ++i)
		std::getline(is, tmp[i], ',');

	v.x = Converter::convert<float>(tmp[0]);
	v.y = Converter::convert<float>(tmp[1]);
	v.z = Converter::convert<float>(tmp[2]);

	return is;
}