#include "Plane.h"

Plane Plane::p;

Plane::Plane() : D3DXPLANE(0, 0, 0, 0) {}
Plane::Plane(const D3DXPLANE& p) : D3DXPLANE(p) {}


Plane Plane::createFromPointNormal(const Vector3& point, const Vector3 normal)
{
	return *::D3DXPlaneFromPointNormal(&p, &point, &normal);
}


void Plane::transform(const Matrix& m)
{
	::D3DXPlaneTransform(this, this, &m);
}


void Plane::normalize()
{
	::D3DXPlaneNormalize(this, this);
}


Plane Plane::transform(const Plane& p, const Matrix& m)
{
	return *::D3DXPlaneTransform(&Plane::p, &p, &m);
}


float Plane::dotCoord(const Vector3& v) const
{
	return ::D3DXPlaneDotCoord(this, &v);
}


void Plane::set(unsigned index, float value)
{
	switch (index)
	{
		case 0: a = value; break;
		case 1: b = value; break;
		case 2: c = value; break;
		case 3: d = value; break;
		default: throw std::exception();
	}
}