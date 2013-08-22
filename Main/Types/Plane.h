#pragma once

#include <d3dx9.h>

#include "Vector3.h"
#include "Matrix.h"

struct Plane : public D3DXPLANE
{
	Plane();
	Plane(const D3DXPLANE& p);

	void transform(const Matrix& m);
	float dotCoord(const Vector3& v) const;
	void normalize();

	static Plane createFromPointNormal(const Vector3& point, const Vector3 normal);
	static Plane transform(const Plane& p, const Matrix& m);

	void set(unsigned index, float value);

private:
	static Plane p;
};