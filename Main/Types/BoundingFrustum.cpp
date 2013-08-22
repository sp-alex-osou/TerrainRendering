#include "BoundingFrustum.h"

#include "BoundingBox.h"


BoundingFrustum::BoundingFrustum()
{
}


BoundingFrustum::BoundingFrustum(const Matrix& viewProjection)
{
	for (unsigned i = 0; i < 4; ++i)
	{
		leftPlane.set(i, viewProjection.m[i][3] + viewProjection.m[i][0]);
		rightPlane.set(i, viewProjection.m[i][3] - viewProjection.m[i][0]);
		bottomPlane.set(i, viewProjection.m[i][3] + viewProjection.m[i][1]);
		topPlane.set(i, viewProjection.m[i][3] - viewProjection.m[i][1]);
		nearPlane.set(i, viewProjection.m[i][2]);
		farPlane.set(i, viewProjection.m[i][3] - viewProjection.m[i][2]);
	}

	for (unsigned i = 0; i < 6; ++i)
		(*this)[i].normalize();
}


unsigned BoundingFrustum::contains(const BoundingBox& boundingBox) const
{
	unsigned result = ContainmentType::contains;

	for (unsigned i = 0; i < 6; ++i)
	{
		bool allIn = true;
		bool allOut = true;

		for (unsigned j = 0; j < 8 && (allIn || allOut); ++j)
		{
			bool in = (*this)[i].dotCoord(boundingBox[j]) > 0;

			if (in)
				allOut = false;
			else
				allIn = false;
		}

		if (allOut)
			return ContainmentType::disjoint;

		if (!allIn)
			result = ContainmentType::intersects;
	}

	return result;
}


const Plane& BoundingFrustum::operator[](unsigned index) const
{
	switch (index)
	{
		case 0: return nearPlane;
		case 1: return farPlane;
		case 2: return leftPlane;
		case 3: return rightPlane;
		case 4: return topPlane;
		case 5: return bottomPlane;
		default: throw std::exception();
	}
}


Plane& BoundingFrustum::operator[](unsigned index)
{
	switch (index)
	{
		case 0: return nearPlane;
		case 1: return farPlane;
		case 2: return leftPlane;
		case 3: return rightPlane;
		case 4: return topPlane;
		case 5: return bottomPlane;
		default: throw std::exception();
	}
}