#pragma once

#include <D3D11.h>

#include "..\Input\Keyboard.h"
#include "..\Terrain\Terrain.h"

class Game;

class Accuracy
{
	struct Texel
	{
		unsigned char rgba[4];
	};

public:
	void initReferenceData(ID3D11Device* graphicsDevice, Game* game);

	float getAccuracy(ID3D11Device* graphicsDevice, Game* game, bool compareRoughness = false);

	void onResizedSwapChain(ID3D11Device* graphicsDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
	void onReleasingSwapChain();

private:
	void initData(ID3D11Device* graphicsDevice, Texel* data);

	bool isEqual(Texel t0, Texel t1);

	ID3D11Texture2D* renderTargetTextureGPU;
	ID3D11Texture2D* renderTargetTextureCPU;

	D3D11_TEXTURE2D_DESC textureDesc;

	ID3D11RenderTargetView* rtvTexture;
	ID3D11RenderTargetView* rtvCurrent;

	Texel* referenceData;
	Texel* sampleData;
};