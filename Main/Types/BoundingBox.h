#pragma once

#include "Vector3.h"

class BoundingBox
{
public:
	BoundingBox();
	BoundingBox(const Vector3& min, const Vector3& max);

	Vector3 operator[](unsigned index) const;

private:
	Vector3 min;
	Vector3 max;
};