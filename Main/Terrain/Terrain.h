#pragma once

#include <D3D11.h>

#include <D3DX10.h>
#include <D3DX11.h>

#include "TerrainPatch.h"
#include "TerrainEffect.h"
#include "TerrainHeightmap.h"

#include "..\Types\BoundingBox.h"
#include "..\Types\BoundingFrustum.h"

class Terrain
{
	struct Instance
	{
		float col;
		float row;
		float size;
		unsigned textureIndex;
	};

public:
	Terrain();

	void init(ID3D11Device* graphicsDevice);
	void release();

	void update(const D3DXVECTOR3& cameraPos, const BoundingFrustum& viewFrustum);
	void draw(ID3D11Device* graphicsDevice, const D3DXMATRIX& world, const D3DXMATRIX& view, const D3DXMATRIX& projection);

	void setScreenSize(unsigned width, unsigned height) { effect.setScreenSize(width, height); }
	void setFakeView(const D3DXMATRIX& view) { effect.setFakeView(view); }

	float getHeight(const D3DXVECTOR3& position) { return heightmap.getHeight(position) * bumpiness; }

	void setColormapEnabled(bool enabled) { effect.setColormapEnabled(enabled); this->colormapEnabled = enabled; }
	void setLightingEnabled(bool enabled) { effect.setLightingEnabled(enabled); this->lightingEnabled = enabled; }
	void setHeightTextureEnabled(bool enabled) { effect.setHeightTextureEnabled(enabled); this->heightTextureEnabled = enabled; }
	void setSlopeTextureEnabled(bool enabled) { effect.setSlopeTextureEnabled(enabled); this->slopeTextureEnabled = enabled; }
	void setRoughnessEnabled(bool enabled) { effect.setRoughnessEnabled(enabled); this->roughnessEnabled = enabled; }
	void setAntiShimmeringEnabled(bool enabled) { effect.setAntiShimmeringEnabled(enabled); this->antiShimmeringEnabled = enabled; }
	void setShowNodesEnabled(bool enabled) { effect.setShowNodesEnabled(enabled); this->showNodesEnabled = enabled; }
	void setGPUFrustumCullingEnabled(bool enabled) { effect.setFrustumCullingEnabled(enabled); this->gpuFrustumCullingEnabled = enabled; }
	void setBruteForceEnabled(bool enabled) { effect.setBruteForceEnabled(enabled); this->bruteForceEnabled = enabled; }
	void setCPUFrustumCullingEnabled(bool enabled) { this->cpuFrustumCullingEnabled = enabled; }
	void setUniformPatchSizeEnabled(bool enabled) { this->uniformPatchSizeEnabled = enabled; }

	void setMinPixelPerTriangle(float minPixelPerTriangle) { effect.setMinPixelPerTriangle(minPixelPerTriangle); this->minPixelPerTriangle = minPixelPerTriangle; }
	void setBumpiness(float bumpiness) { effect.setBumpiness(bumpiness); this->bumpiness = bumpiness; }
	void setRoughnessModificator(float roughnessModificator) { effect.setRoughnessModificator(roughnessModificator); this->roughnessModificator = roughnessModificator; }
	void setColormapRepeat(float colormapRepeat) { effect.setColormapRepeat(colormapRepeat); this->colormapRepeat = colormapRepeat; }

	bool getColormapEnabled() const { return colormapEnabled; }
	bool getLightingEnabled() const { return lightingEnabled; }
	bool getHeightTextureEnabled() const { return heightTextureEnabled; }
	bool getSlopeTextureEnabled() const { return slopeTextureEnabled; }
	bool getCPUFrustumCullingEnabled() const { return cpuFrustumCullingEnabled; }
	bool getGPUFrustumCullingEnabled() const { return gpuFrustumCullingEnabled; }
	bool getShowNodesEnabled() const { return showNodesEnabled; }
	bool getRoughnessEnabled() const { return roughnessEnabled; }
	bool getAntiShimmeringEnabled() const { return antiShimmeringEnabled; }
	bool getUniformPatchSizeEnabled() const { return uniformPatchSizeEnabled; }
	bool getBruteForceEnabled() const { return bruteForceEnabled; }

	float getMinPixelPerTriangle() const { return minPixelPerTriangle; }
	float getColormapRepeat() const { return colormapRepeat; }
	float getBumpiness() const { return bumpiness; }
	float getRoughnessModificator() const { return roughnessModificator; }
	
	unsigned getPatchSize() const { return patchSize; }
	unsigned getNodeSize() const { return nodeSize; }

	unsigned getWidth() const { return numCols; }
	unsigned getLength() const { return numRows; }

private:  
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* instanceBuffer;

	Instance* instances;

	unsigned numInstances;
	unsigned numNodes;

	float terrainScale;

	static const unsigned terrainSize;
	static const unsigned nodeSize;
	static const unsigned patchSize;

	static const char* roughnessFilePath;
	static const char* colorinfoFilePath;

	float bumpiness;
	float roughnessModificator;
	float minPixelPerTriangle;
	float colormapRepeat;

	bool colormapEnabled;
	bool lightingEnabled;
	bool slopeTextureEnabled;
	bool heightTextureEnabled;
	bool cpuFrustumCullingEnabled;
	bool gpuFrustumCullingEnabled;
	bool roughnessEnabled;
	bool antiShimmeringEnabled;
	bool showNodesEnabled;
	bool uniformPatchSizeEnabled;
	bool bruteForceEnabled;

	unsigned numRows;
	unsigned numCols;

	unsigned numPatchesPerNode;
	unsigned numPatchRows;
	unsigned numPatchCols;

	TerrainEffect effect;
	TerrainHeightmap heightmap;

	void updateNode(unsigned row, unsigned col, unsigned size, unsigned& numInstances, const D3DXVECTOR3& cameraPos, const BoundingFrustum& viewFrustum, bool frustumCulling);

	void initPatchRoughness(float** patchRoughness, unsigned numLevels) const;
	void initVertexBuffer(ID3D11Device* graphicsDevice);
	void initInstanceBuffer(ID3D11Device* graphicsDevice);

	ID3D11ShaderResourceView* loadRoughnessmap(ID3D11Device* graphicsDevice) const;
	ID3D11ShaderResourceView* loadInfomap(ID3D11Device* graphicsDevice, const D3DXVECTOR2* ranges, unsigned char numRanges) const;
	ID3D11ShaderResourceView* loadTextureArray(ID3D11Device* graphicsDevice, const char** textureNames, unsigned numTextures) const;

	ID3D11ShaderResourceView* createTextureArray(ID3D11Device* graphicsDevice, ID3D11Texture2D** textures, unsigned numTextures) const;
	
	float getPatchRoughness(unsigned patchRow, unsigned patchCol) const;
	
	int clamp(int value, int min, int max) const;
	unsigned getPatchIndex(unsigned patchRow, unsigned patchCol, unsigned numPatchRows, unsigned numPatchCols) const;
};
