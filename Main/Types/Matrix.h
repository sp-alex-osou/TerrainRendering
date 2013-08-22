#pragma once

#include <d3dx9math.h>

#include "Vector3.h"
#include "Quaternion.h"

struct Plane;

struct Matrix : public D3DXMATRIX
{
	Matrix();
	Matrix(const D3DXMATRIX& m);

	void decompose(Vector3* scale, Quaternion* rotation, Vector3* translation) const;

	static Matrix createOrthographic(float width, float height, float nearPlaneDistance, float farPlaneDistance);
	static Matrix createPerspectiveFieldOfView(float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance);
	static Matrix createLookAt(const Vector3& cameraPosition, const Vector3& cameraTarget, const Vector3& cameraUpVector);
	static Matrix createFromQuaternion(const Quaternion& quaternion);
	static Matrix createTranslation(const Vector3& translation);
	static Matrix createTranslation(float x, float y, float z);
	static Matrix createScale(const Vector3& scale);
	static Matrix createRotation(const Vector3& rotation);
	static Matrix createFromAxisAngle(const Vector3& axis, float angle);
	static Matrix createReflection(const Plane& plane);

	static Matrix invert(const Matrix& matrix);
	static Matrix transpose(const Matrix& matrix);

	static const Matrix identity;

private:
	static Matrix tmp;
};