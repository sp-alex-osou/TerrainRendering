#pragma once

#include <D3DX11.h>
#include <D3DX10.h>

#include <d3dx11effect.h>

#include <exception>

#include "TerrainPatch.h"

struct IntVector2
{
	int x;
	int y;

	IntVector2(int x, int y) : x(x), y(y) {}

	operator const int*() { return (const int*)this; }
};

class TerrainEffect
{
public:
	void init(ID3D11Device* graphicsDevice, const char* path);
	void release();

	void setWorldViewProjection(const D3DXMATRIX& world, const D3DXMATRIX& view, const D3DXMATRIX& projection);
	void setFakeView(const D3DXMATRIX& view);

	void setColormap(ID3D11ShaderResourceView* colormapView) { this->colormap->SetResource(colormapView); }
	void setNormalmap(ID3D11ShaderResourceView* normalmapView) { this->normalmap->SetResource(normalmapView); }
	void setHeightmap(ID3D11ShaderResourceView* heightmapView) { this->heightmap->SetResource(heightmapView); }
	void setColormaps(ID3D11ShaderResourceView* colormapsView) { this->colormaps->SetResource(colormapsView); }
	void setInfomap(ID3D11ShaderResourceView* infomapView) { this->infomap->SetResource(infomapView); }
	void setSlopemap(ID3D11ShaderResourceView* slopemapView) { this->slopemap->SetResource(slopemapView); }
	void setRoughnessmaps(ID3D11ShaderResourceView* roughnessmapsView) { this->roughnessmaps->SetResource(roughnessmapsView); }

	void setTerrainScale(float terrainScale) { this->terrainScale->SetFloat(terrainScale); }
	void setMinPatchSize(unsigned minPatchSize) { this->minPatchSize->SetInt(minPatchSize); }
	void setMinNodeSize(unsigned minNodeSize) { this->minNodeSize->SetInt(minNodeSize); }

	void setBumpiness(float bumpiness) { this->bumpiness->SetFloat(bumpiness); }
	void setRoughnessModificator(float roughnessModificator) { this->roughnessModificator->SetFloat(roughnessModificator); }
	void setMinPixelPerTriangle(float minPixelPerTriangle) { this->minPixelPerTriangle->SetFloat(minPixelPerTriangle); }
	void setColormapRepeat(float colormapRepeat) { this->colormapRepeat->SetFloat(colormapRepeat); }

	void setTerrainSize(unsigned size) { this->terrainSize->SetInt(size); }
	void setScreenSize(unsigned width, unsigned height) { this->screenSize->SetIntVector(IntVector2(width, height)); }
	void setSlopeRange(const D3DXVECTOR2& range) { this->slopeRange->SetFloatVector(range); }

	void setColormapEnabled(bool enabled) { this->colormapEnabled->SetBool(enabled); }
	void setLightingEnabled(bool enabled) { this->lightingEnabled->SetBool(enabled); }
	void setHeightTextureEnabled(bool enabled) { this->heightTextureEnabled->SetBool(enabled); }
	void setSlopeTextureEnabled(bool enabled) { this->slopeTextureEnabled->SetBool(enabled); }
	void setFrustumCullingEnabled(bool enabled) { this->frustumCullingEnabled->SetBool(enabled); }
	void setRoughnessEnabled(bool enabled) { this->roughnessEnabled->SetBool(enabled); }
	void setAntiShimmeringEnabled(bool enabled) { this->antiShimmeringEnabled->SetBool(enabled); }
	void setShowNodesEnabled(bool enabled) { this->showNodesEnabled->SetBool(enabled); }
	void setBruteForceEnabled(bool enabled) { this->bruteForceEnabled->SetBool(enabled); }
	
	ID3DX11EffectPass* getEffectPass() { return effectPass; }
	ID3D11InputLayout* getInputLayout() { return inputLayout; }

private:
	ID3DX11Effect* effect;
	ID3DX11EffectPass* effectPass;

	ID3D11InputLayout* inputLayout;

	ID3DX11EffectShaderResourceVariable* colormap;
	ID3DX11EffectShaderResourceVariable* normalmap;
	ID3DX11EffectShaderResourceVariable* heightmap;
	ID3DX11EffectShaderResourceVariable* colormaps;
	ID3DX11EffectShaderResourceVariable* roughnessmaps;
	ID3DX11EffectShaderResourceVariable* slopemap;
	ID3DX11EffectShaderResourceVariable* infomap;

	ID3DX11EffectMatrixVariable* world;
	ID3DX11EffectMatrixVariable* projection;
	ID3DX11EffectMatrixVariable* view;

	ID3DX11EffectMatrixVariable* fakeView;

	ID3DX11EffectMatrixVariable* worldViewProjection;

	ID3DX11EffectScalarVariable* colormapEnabled;
	ID3DX11EffectScalarVariable* heightTextureEnabled;
	ID3DX11EffectScalarVariable* slopeTextureEnabled;
	ID3DX11EffectScalarVariable* lightingEnabled;
	ID3DX11EffectScalarVariable* frustumCullingEnabled;
	ID3DX11EffectScalarVariable* roughnessEnabled;
	ID3DX11EffectScalarVariable* antiShimmeringEnabled;
	ID3DX11EffectScalarVariable* showNodesEnabled;
	ID3DX11EffectScalarVariable* bruteForceEnabled;

	ID3DX11EffectScalarVariable* bumpiness;
	ID3DX11EffectScalarVariable* minPixelPerTriangle;
	ID3DX11EffectScalarVariable* colormapRepeat;
	ID3DX11EffectScalarVariable* roughnessModificator;

	ID3DX11EffectScalarVariable* minPatchSize;
	ID3DX11EffectScalarVariable* minNodeSize;
	ID3DX11EffectScalarVariable* terrainScale;
	ID3DX11EffectScalarVariable* terrainSize;

	ID3DX11EffectVectorVariable* screenSize;
	ID3DX11EffectVectorVariable* slopeRange;
};
