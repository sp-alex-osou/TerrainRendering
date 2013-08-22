#include "BoundingBox.h"

#undef min
#undef max


BoundingBox::BoundingBox()
{
}


BoundingBox::BoundingBox(const Vector3& min, const Vector3& max) : min(min), max(max)
{
}


Vector3 BoundingBox::operator[](unsigned index) const
{
	switch (index)
	{
		case 0: return Vector3(min.x, min.y, min.z); break;
		case 1: return Vector3(min.x, min.y, max.z); break;
		case 2: return Vector3(min.x, max.y, min.z); break;
		case 3: return Vector3(min.x, max.y, max.z); break;
		case 4: return Vector3(max.x, min.y, min.z); break;
		case 5: return Vector3(max.x, min.y, max.z); break;
		case 6: return Vector3(max.x, max.y, min.z); break;
		case 7: return Vector3(max.x, max.y, max.z); break;
		default: throw std::exception();
	}
}