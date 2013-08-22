#include "TerrainHeightmap.h"

#include <numeric>

#undef min
#undef max

void TerrainHeightmap::init(ID3D11Device* graphicsDevice, const char* path)
{
	ID3D11Resource* heightmap;
	ID3D11Texture2D* texture;

	D3DX11_IMAGE_LOAD_INFO info;

	info.Width = D3DX11_DEFAULT;
	info.Height = D3DX11_DEFAULT;
	info.Depth = D3DX11_DEFAULT;
	info.FirstMipLevel = D3DX11_DEFAULT;
	info.MipLevels = D3DX11_DEFAULT;
	info.Usage = (D3D11_USAGE) D3D11_USAGE_STAGING;
	info.BindFlags = 0;
	info.CpuAccessFlags = D3D11_CPU_ACCESS_READ;
	info.MiscFlags = D3DX11_DEFAULT;
	info.Format = DXGI_FORMAT_FROM_FILE;
	info.Filter = D3DX11_DEFAULT;
	info.MipFilter = D3DX11_DEFAULT;
	info.pSrcInfo = NULL;

	D3DX11CreateTextureFromFileA(graphicsDevice, path, &info, 0, &heightmap, 0);

	heightmap->QueryInterface(&texture);

	D3D11_TEXTURE2D_DESC desc;

	texture->GetDesc(&desc);

	numRows = desc.Height;
	numCols = desc.Width;

	unsigned elements = numRows * numCols;

	heights = new float[elements];
	
	ID3D11DeviceContext* immediateContext;

	graphicsDevice->GetImmediateContext(&immediateContext);

	D3D11_MAPPED_SUBRESOURCE subresource;

	immediateContext->Map(heightmap, 0, D3D11_MAP_READ, 0, &subresource);

	unsigned count = 0;

	for (unsigned row = 0; row < numRows; ++row)
	{
		D3DXFLOAT16* data = (D3DXFLOAT16*)((char*)subresource.pData + subresource.RowPitch * row);

		for (unsigned col = 0; col < numCols; ++col)
			heights[count++] = data[col];
	}

	immediateContext->Unmap(heightmap, 0);

	immediateContext->Release();

	heightmap->Release();
	texture->Release();
}


void TerrainHeightmap::release()
{
	delete[] heights;
}


float TerrainHeightmap::getHeight(unsigned row, unsigned col) const
{
	return heights[getIndex(row, col)];
}


float TerrainHeightmap::getHeight(const D3DXVECTOR3& position) const
{
	float x = clamp(position.x, 0.0f, (float)numCols);
	float z = clamp(position.z, 0.0f, (float)numRows);

	// quad indices
	unsigned rLow = (unsigned)std::floor(z);
	unsigned rHigh = (unsigned)std::ceil(z);
	unsigned cLow = (unsigned)std::floor(x);
	unsigned cHigh = (unsigned)std::ceil(x);

	// quad heights
	float h00 = getHeight(rLow, cLow);
	float h01 = getHeight(rLow, cHigh);
	float h10 = getHeight(rHigh, cLow);
	float h11 = getHeight(rHigh, cHigh);

	// coordinates inside quad
	float r = z - rLow;
	float c = x - cLow;

	// bilinear interpolation
	float h0 = h00 * (1.0f - c) + h01 * c;
	float h1 = h10 * (1.0f - c) + h11 * c;

	return h0 * (1.0f - r) + h1 * r;
}


unsigned TerrainHeightmap::getIndex(unsigned row, unsigned col) const
{
	//row = clamp(row, 0U, numRows - 1);
	//col = clamp(col, 0U, numCols - 1);

	return (numRows - 1 - row) * numRows + col;
}