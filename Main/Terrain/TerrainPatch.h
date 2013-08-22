#pragma once

#include <D3D11.h>
#include <D3DX10.h>

struct TerrainPatch
{
	D3DXVECTOR2 position;

	static D3D11_INPUT_ELEMENT_DESC inputElementDesc[];
	static unsigned numElements;
};