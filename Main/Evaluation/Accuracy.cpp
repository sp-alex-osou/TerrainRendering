#include "Accuracy.h"
#include "..\Game.h"


void Accuracy::initReferenceData(ID3D11Device* graphicsDevice, Game* game)
{
	bool bruteForceEnabled = game->terrain->getBruteForceEnabled();
	ID3D11RenderTargetView* rtvDefault = game->renderTargetView;

	game->renderTargetView = rtvTexture;
	game->terrain->setBruteForceEnabled(true);

	game->drawTerrain(graphicsDevice);

	game->renderTargetView = rtvDefault;
	game->terrain->setBruteForceEnabled(bruteForceEnabled);

	initData(graphicsDevice, referenceData);
}


float Accuracy::getAccuracy(ID3D11Device* graphicsDevice, Game* game, bool compareRoughness)
{
	ID3D11DeviceContext* immediateContext;

	graphicsDevice->GetImmediateContext(&immediateContext);

	ID3D11RenderTargetView* rtvDefault = game->renderTargetView;

	game->renderTargetView = rtvTexture;

	if (compareRoughness)
	{
		game->terrain->setRoughnessEnabled(false);

		game->drawTerrain(graphicsDevice);
		initData(graphicsDevice, referenceData);

		game->terrain->setRoughnessEnabled(true);
	}

	game->drawTerrain(graphicsDevice);
	initData(graphicsDevice, sampleData);

	game->renderTargetView = rtvDefault;

	unsigned total = 0;
	unsigned equal = 0;

	for (unsigned i = 0; i < textureDesc.Width * textureDesc.Height; ++i)
	{
		if (isEqual(sampleData[i], referenceData[i]))
			++equal;

		++total;
	}

	return (float)equal / (float)total;
}


void Accuracy::onResizedSwapChain(ID3D11Device* graphicsDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	textureDesc.Width = pBackBufferSurfaceDesc->Width;
	textureDesc.Height = pBackBufferSurfaceDesc->Height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = pBackBufferSurfaceDesc->Format;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.MiscFlags = 0;

	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;

	graphicsDevice->CreateTexture2D(&textureDesc, 0, &renderTargetTextureGPU);

	textureDesc.Usage = D3D11_USAGE_STAGING;
	textureDesc.BindFlags = 0;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	graphicsDevice->CreateTexture2D(&textureDesc, 0, &renderTargetTextureCPU);

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;

	rtvDesc.Format = textureDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	graphicsDevice->CreateRenderTargetView(renderTargetTextureGPU, &rtvDesc, &rtvTexture);

	sampleData = new Texel[textureDesc.Height * textureDesc.Width];
	referenceData = new Texel[textureDesc.Height * textureDesc.Width];
}


void Accuracy::onReleasingSwapChain()
{
	renderTargetTextureGPU->Release();
	renderTargetTextureCPU->Release();
	rtvTexture->Release();

	delete[] sampleData;
	delete[] referenceData;
}


bool Accuracy::isEqual(Texel t0, Texel t1)
{
	for (unsigned i = 0; i < 4; ++i)
		if (std::abs((int)t0.rgba[i] - (int)t1.rgba[i]) > 1)
			return false;

	return true;
}


void Accuracy::initData(ID3D11Device* graphicsDevice, Texel* data)
{
	ID3D11DeviceContext* immediateContext;

	graphicsDevice->GetImmediateContext(&immediateContext);

	immediateContext->CopyResource(renderTargetTextureCPU, renderTargetTextureGPU);

	D3D11_MAPPED_SUBRESOURCE subresource;

	immediateContext->Map(renderTargetTextureCPU, 0, D3D11_MAP_READ, 0, &subresource);

	unsigned count = 0;

	for (unsigned row = 0; row < textureDesc.Height; ++row)
	{
		Texel* sample = (Texel*)((char*)subresource.pData + subresource.RowPitch * row);
	
		for (unsigned col = 0; col < textureDesc.Width; ++col)
			data[count++] = sample[col];
	}

	immediateContext->Unmap(renderTargetTextureCPU, 0);

	immediateContext->Release();
}