#pragma once

#include <D3D11.h>
#include <D3DX10.h>
#include <D3DX11.h>

class TerrainHeightmap
{
public:
	void init(ID3D11Device* graphicsDevice, const char* path);
	void release();

	float getHeight(unsigned row, unsigned col) const;
	float getHeight(const D3DXVECTOR3& position) const;

	unsigned getNumRows() const { return numRows; }
	unsigned getNumCols() const { return numCols; }

private:
	float* heights;

	unsigned numRows;
	unsigned numCols;

	unsigned getIndex(unsigned row, unsigned col) const;
	
	template <class T> T clamp(T f, T a, T b) const { return (f < a) ? a : (f > b) ? b : f; }
};