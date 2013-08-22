#pragma once

#include <d3dx9math.h>

#include "Vector3.h"

struct Matrix;

struct Quaternion : public D3DXQUATERNION
{
	Quaternion();
	Quaternion(const D3DXQUATERNION& q);

	static Quaternion createFromAxisAngle(const Vector3& axis, float angle);
	static Quaternion createFromMatrix(const Matrix& m);

private:
	static Quaternion q;
};