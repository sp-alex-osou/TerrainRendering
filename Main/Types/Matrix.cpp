#include "Matrix.h"
#include "Plane.h"

Matrix Matrix::tmp;

const Matrix Matrix::identity = Matrix();

Matrix::Matrix() 
{ 
	D3DXMatrixIdentity(this); 
}


Matrix::Matrix(const D3DXMATRIX& m) : D3DXMATRIX(m) 
{
}


Matrix Matrix::createOrthographic(float w, float h, float zn, float zf) 
{ 
	return *D3DXMatrixOrthoLH(&tmp, w, h, zn, zf); 
}


Matrix Matrix::createLookAt(const Vector3& pEye, const Vector3& pAt, const Vector3& pUp) 
{ 
	return *D3DXMatrixLookAtLH(&tmp, &pEye, &pAt, &pUp);
}


Matrix Matrix::createFromQuaternion(const Quaternion& q)
{
	return *D3DXMatrixRotationQuaternion(&tmp, &q);
}


Matrix Matrix::createTranslation(const Vector3& v)
{
	return *D3DXMatrixTranslation(&tmp, v.x, v.y, v.z);
}

Matrix Matrix::createTranslation(float x, float y, float z)
{
	return *D3DXMatrixTranslation(&tmp, x, y, z);
}

Matrix Matrix::createScale(const Vector3& v)
{
	return *D3DXMatrixScaling(&tmp, v.x, v.y, v.z);
}


Matrix Matrix::createRotation(const Vector3& v)
{
	return *D3DXMatrixRotationYawPitchRoll(&tmp, v.y, v.x, v.z);
}


Matrix Matrix::createPerspectiveFieldOfView(float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance)
{
	return *D3DXMatrixPerspectiveFovLH(&tmp, fieldOfView, aspectRatio, nearPlaneDistance, farPlaneDistance);
}


Matrix Matrix::invert(const Matrix& matrix)
{
	return *D3DXMatrixInverse(&tmp, 0, &matrix);
}


Matrix Matrix::transpose(const Matrix& matrix)
{
	return *D3DXMatrixTranspose(&tmp, &matrix);
}


Matrix Matrix::createFromAxisAngle(const Vector3& axis, float angle)
{
	return *D3DXMatrixRotationAxis(&tmp, &axis, angle);
}


void Matrix::decompose(Vector3* scale, Quaternion* rotation, Vector3* translation) const
{
	D3DXMatrixDecompose(scale, rotation, translation, this);
}


Matrix Matrix::createReflection(const Plane& plane)
{
	return *D3DXMatrixReflect(&tmp, &plane);
}