#pragma once

#include "Matrix.h"
#include "Plane.h"
#include "ContainmentType.h"
#include "BoundingBox.h"

class BoundingFrustum
{
public:
	BoundingFrustum();
	BoundingFrustum(const Matrix& viewProjection);

	const Plane& operator[](unsigned index) const;
	Plane& operator[](unsigned index);

	unsigned contains(const BoundingBox& boundingBox) const;

private:
	Plane nearPlane;
	Plane farPlane;
	Plane leftPlane;
	Plane rightPlane;
	Plane topPlane;
	Plane bottomPlane;
};