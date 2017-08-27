#include "TerrainEffect.h"

#include <D3Dcompiler.h>

void TerrainEffect::init(ID3D11Device* graphicsDevice, const char* path)
{
	ID3DBlob* errorBlob;
	ID3DBlob* effectBlob;

	#if defined(DEBUG) | defined(_DEBUG)
		unsigned flags = D3DCOMPILE_DEBUG | D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_WARNINGS_ARE_ERRORS;
	#else
		unsigned flags = D3DCOMPILE_OPTIMIZATION_LEVEL3;
	#endif

	HRESULT hr = D3DX11CompileFromFileA(path, 0, 0, 0, "fx_5_0", flags, 0, 0, &effectBlob, &errorBlob, 0);

	if (hr != S_OK)
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		throw std::exception();
	}

	D3DX11CreateEffectFromMemory(effectBlob->GetBufferPointer(), effectBlob->GetBufferSize(), 0, graphicsDevice, &effect);

	effectBlob->Release();

	ID3DX11EffectTechnique* technique;
	
	technique = effect->GetTechniqueByIndex(0);
	effectPass = technique->GetPassByIndex(0);

	D3DX11_PASS_SHADER_DESC vertexShaderDesc;
	D3DX11_EFFECT_SHADER_DESC effectShaderDesc;
	
	effectPass->GetVertexShaderDesc(&vertexShaderDesc);

	vertexShaderDesc.pShaderVariable->GetShaderDesc(vertexShaderDesc.ShaderIndex, &effectShaderDesc);

	graphicsDevice->CreateInputLayout(TerrainPatch::inputElementDesc, TerrainPatch::numElements, effectShaderDesc.pBytecode, effectShaderDesc.BytecodeLength, &inputLayout);

	colormap = effect->GetVariableByName("Colormap")->AsShaderResource();
	heightmap = effect->GetVariableByName("Heightmap")->AsShaderResource();
	normalmap = effect->GetVariableByName("Normalmap")->AsShaderResource();
	colormaps = effect->GetVariableByName("Colormaps")->AsShaderResource();
	infomap = effect->GetVariableByName("Infomap")->AsShaderResource();
	slopemap = effect->GetVariableByName("Slopemap")->AsShaderResource();
	roughnessmap = effect->GetVariableByName("Roughnessmap")->AsShaderResource();

	fakeView = effect->GetVariableByName("FakeView")->AsMatrix();

	world = effect->GetVariableByName("World")->AsMatrix();
	view = effect->GetVariableByName("View")->AsMatrix();
	projection = effect->GetVariableByName("Projection")->AsMatrix();

	worldViewProjection = effect->GetVariableByName("WorldViewProjection")->AsMatrix();

	terrainSize = effect->GetVariableByName("TerrainSize")->AsScalar();
	terrainScale = effect->GetVariableByName("TerrainScale")->AsScalar();
	minPatchSize = effect->GetVariableByName("MinPatchSize")->AsScalar();
	minNodeSize = effect->GetVariableByName("MinNodeSize")->AsScalar();

	bumpiness = effect->GetVariableByName("Bumpiness")->AsScalar();
	roughnessModificator = effect->GetVariableByName("RoughnessModificator")->AsScalar();
	minPixelPerTriangle = effect->GetVariableByName("MinPixelPerTriangle")->AsScalar();
	colormapRepeat = effect->GetVariableByName("ColormapRepeat")->AsScalar();

	screenSize = effect->GetVariableByName("ScreenSize")->AsVector();
	slopeRange = effect->GetVariableByName("SlopeRange")->AsVector();

	colormapEnabled = effect->GetVariableByName("ColormapEnabled")->AsScalar();
	frustumCullingEnabled = effect->GetVariableByName("FrustumCullingEnabled")->AsScalar();
	heightTextureEnabled = effect->GetVariableByName("HeightTextureEnabled")->AsScalar();
	slopeTextureEnabled = effect->GetVariableByName("SlopeTextureEnabled")->AsScalar();
	lightingEnabled = effect->GetVariableByName("LightingEnabled")->AsScalar();
	roughnessEnabled = effect->GetVariableByName("RoughnessEnabled")->AsScalar();
	antiShimmeringEnabled = effect->GetVariableByName("AntiShimmeringEnabled")->AsScalar();
	showNodesEnabled = effect->GetVariableByName("ShowNodesEnabled")->AsScalar();
	bruteForceEnabled = effect->GetVariableByName("BruteForceEnabled")->AsScalar();
}


void TerrainEffect::release()
{
	effect->Release();
	inputLayout->Release();
}


void TerrainEffect::setWorldViewProjection(const D3DXMATRIX& world, const D3DXMATRIX& view, const D3DXMATRIX& projection)
{
	this->worldViewProjection->SetMatrix(world * view * projection);

	this->world->SetMatrix(world);
	this->view->SetMatrix(view);
	this->projection->SetMatrix(projection);
}


void TerrainEffect::setFakeView(const D3DXMATRIX& view)
{
	this->fakeView->SetMatrix(view);
}
