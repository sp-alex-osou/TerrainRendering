#include "Quaternion.h"
#include "Matrix.h"

Quaternion Quaternion::q;

Quaternion::Quaternion() : D3DXQUATERNION(0, 0, 0, 1) {}
Quaternion::Quaternion(const D3DXQUATERNION& q) : D3DXQUATERNION(q) {}

Quaternion Quaternion::createFromAxisAngle(const Vector3& axis, float angle) 
{ 
	return *::D3DXQuaternionRotationAxis(&q, &axis, angle);
}


Quaternion Quaternion::createFromMatrix(const Matrix& m) 
{
	return *::D3DXQuaternionRotationMatrix(&q, &m); 
}