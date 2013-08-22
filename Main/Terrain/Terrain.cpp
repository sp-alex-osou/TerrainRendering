#include "Terrain.h"

#include <algorithm>
#include <fstream>

#include "..\Helpers\Console.h"
#include "..\Input\Keyboard.h"

#undef min
#undef max

const char* Terrain::roughnessFilePath = "TerrainRoughness.bin";
const char* Terrain::colorinfoFilePath = "TerrainColorInfo.dds";


Terrain::Terrain()
{
}


void Terrain::init(ID3D11Device* graphicsDevice)
{
	heightmap.init(graphicsDevice, "Content/Textures/Heightmap.dds");
	effect.init(graphicsDevice, "Content/Effects/Terrain.fx");

	numRows = (heightmap.getNumRows() / 2) * 2;
	numCols = (heightmap.getNumCols() / 2) * 2;
	
	terrainScale = (float)terrainSize / (float)std::max(numCols, numRows);

	setBumpiness(100.0f);
	setMinPixelPerTriangle(3.0f);
	setColormapRepeat(128.0f);
	setRoughnessModificator(1.0f);

	setLightingEnabled(true);
	setCPUFrustumCullingEnabled(true);
	setGPUFrustumCullingEnabled(true);
	setHeightTextureEnabled(true);
	setSlopeTextureEnabled(true);
	setRoughnessEnabled(true);
	setAntiShimmeringEnabled(false); 
	setShowNodesEnabled(false);
	setUniformPatchSizeEnabled(false);
	setBruteForceEnabled(false);
	setColormapEnabled(false);
	
	effect.setTerrainScale(terrainScale);
	effect.setTerrainSize(terrainSize);
	effect.setMinPatchSize(patchSize);
	effect.setMinNodeSize(nodeSize);

	effect.setSlopeRange(D3DXVECTOR2(0.80f, 1.0f));
	
	numPatchRows = numPatchCols = nodeSize / patchSize;
	numPatches = numPatchRows * numPatchCols;
	numNodes = (unsigned)(1.5f * std::pow(std::max(numCols, numRows) / nodeSize, 2.0));

	initVertexBuffer(graphicsDevice);
	initInstanceBuffer(graphicsDevice);

	ID3D11ShaderResourceView* colormap;
	ID3D11ShaderResourceView* heightmap;
	ID3D11ShaderResourceView* normalmap;
	ID3D11ShaderResourceView* colormaps;
	ID3D11ShaderResourceView* slopemap;
	ID3D11ShaderResourceView* infomap;
	ID3D11ShaderResourceView* roughnessmaps;

	nodes = new Node[numNodes];
	instances = new Instance[numNodes];

	unsigned numTotalPatches = (numRows / patchSize) * (numCols / patchSize);

	float* patchRoughness = new float[numTotalPatches];
	ID3D11Texture2D** textures = new ID3D11Texture2D*[(unsigned)(1.5f * numTotalPatches)];

	Console::out << "Calculating Roughness..." << Console::endl;
	initPatchRoughness(patchRoughness);

	unsigned numCurrentNodes = 1;
	unsigned numTextures = 0;

	Console::out << "Initializing Nodes..." << Console::endl;
	initNode(nodes, graphicsDevice, 0, 0, std::max(numRows, numCols), numCurrentNodes, textures, numTextures, patchRoughness);

	Console::out << "Creating Roughness Maps..." << Console::endl;
	roughnessmaps = createTextureArray(graphicsDevice, textures, numTextures);

	for (unsigned i = 0; i < numTextures; i++)
		textures[i]->Release();

	delete[] patchRoughness;
	delete[] textures;

	Console::out << "Loading Heightmap..." << Console::endl;
	D3DX11CreateShaderResourceViewFromFileA(graphicsDevice, "Content/Textures/Heightmap.dds", 0, 0, &heightmap, 0);

	Console::out << "Loading Normalmap..." << Console::endl;
	D3DX11CreateShaderResourceViewFromFileA(graphicsDevice, "Content/Textures/Normalmap.dds", 0, 0, &normalmap, 0);

	Console::out << "Loading Textures..." << Console::endl;
	D3DX11CreateShaderResourceViewFromFileA(graphicsDevice, "Content/Textures/Rock.dds", 0, 0, &slopemap, 0);
	D3DX11CreateShaderResourceViewFromFileA(graphicsDevice, "Content/Textures/Colormap.dds", 0, 0, &colormap, 0);

	const char* textureNames[] = 
	{ 
		"Content/Textures/Sand.dds",
		"Content/Textures/Grass.dds",
		"Content/Textures/Stone.dds",
		"Content/Textures/Snow.dds"
	};

	colormaps = loadTextureArray(graphicsDevice, textureNames, _countof(textureNames));

	const D3DXVECTOR2 colorRanges[] =
	{
		D3DXVECTOR2(0, 0.02f),
		D3DXVECTOR2(0.005f, 0.15f),
		D3DXVECTOR2(0.1f, 0.3f),
		D3DXVECTOR2(0.15f, 1.0f)
	};

	Console::out << "Loading Infomap..." << Console::endl;
	infomap = loadInfomap(graphicsDevice, colorRanges, _countof(colorRanges));

	effect.setColormap(colormap);
	effect.setHeightmap(heightmap);
	effect.setNormalmap(normalmap);
	effect.setColormaps(colormaps);
	effect.setInfomap(infomap);
	effect.setSlopemap(slopemap);
	effect.setRoughnessmaps(roughnessmaps);
	
	heightmap->Release();
	normalmap->Release();
	colormaps->Release();
	infomap->Release();
	slopemap->Release();
	roughnessmaps->Release();
}


void Terrain::release()
{
	effect.release();
	heightmap.release();

	delete[] nodes;
	delete[] instances;

	vertexBuffer->Release();
	instanceBuffer->Release();
}


void Terrain::update(const D3DXVECTOR3& cameraPos, const BoundingFrustum& viewFrustum)
{
	numInstances = 0;

	D3DXVECTOR3 p = cameraPos;

	p.x /= terrainScale;
	p.z /= terrainScale;

	updateNode(nodes, 0, 0, std::max(numRows, numCols), numInstances, p, viewFrustum, cpuFrustumCullingEnabled);
}


void Terrain::draw(ID3D11Device* graphicsDevice, const D3DXMATRIX& world, const D3DXMATRIX& view, const D3DXMATRIX& projection)
{
	ID3D11DeviceContext* immediateContext;

	graphicsDevice->GetImmediateContext(&immediateContext);

	D3D11_MAPPED_SUBRESOURCE subresource;

	immediateContext->Map(instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subresource);
	memcpy(subresource.pData, instances, sizeof(Instance) * numInstances);
	immediateContext->Unmap(instanceBuffer, 0);

	unsigned strides[2] = { sizeof(TerrainPatch), sizeof(Instance) };
	unsigned offsets[2] = { 0, 0 };
	ID3D11Buffer* buffers[2] = { vertexBuffer, instanceBuffer };
	
	immediateContext->IASetInputLayout(effect.getInputLayout());
	immediateContext->IASetVertexBuffers(0, 2, buffers, strides, offsets);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST);

	effect.setWorldViewProjection(world, view, projection);
	effect.getEffectPass()->Apply(0, immediateContext);

	immediateContext->DrawInstanced(numPatches, numInstances, 0, 0);

	immediateContext->Release();
}


void Terrain::initVertexBuffer(ID3D11Device* graphicsDevice)
{
	TerrainPatch* patches = new TerrainPatch[numPatches];

	//unsigned numPatchRows = 1;
	//unsigned numPatchCols = 1;

	unsigned count = 0;

	for (unsigned patchRow = 0; patchRow < numPatchRows; ++patchRow)
		for (unsigned patchCol = 0; patchCol < numPatchCols; ++patchCol)
			patches[count++].position = D3DXVECTOR2((float)patchCol, (float)patchRow);
	
	D3D11_BUFFER_DESC bufferDesc;

	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(TerrainPatch) * numPatches;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;

	initData.pSysMem = patches;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	graphicsDevice->CreateBuffer(&bufferDesc, &initData, &vertexBuffer);

	delete[] patches;
}


void Terrain::initInstanceBuffer(ID3D11Device* graphicsDevice)
{
	D3D11_BUFFER_DESC bufferDesc;

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(Node) * numNodes;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;

	graphicsDevice->CreateBuffer(&bufferDesc, 0, &instanceBuffer);
}


void Terrain::initPatchRoughness(float* patchRoughness)
{
	unsigned numPatchRows = numRows / patchSize;
	unsigned numPatchCols = numCols / patchSize;
	unsigned numPatches = numPatchRows * numPatchCols;

	std::ifstream file(roughnessFilePath, std::ios::binary);

	if (file)
	{
		file.read((char*)patchRoughness, sizeof(float) * numPatches);
		file.close();
		return;
	}

	float maxRoughness = 0.0f;
	unsigned count = 0;
		
	// if roughness file doesn't exist -> create it
	for (unsigned patchRow = 0; patchRow < numPatchRows; ++patchRow)
		for (unsigned patchCol = 0; patchCol < numPatchCols; ++patchCol)
		{
			float roughness = getPatchRoughness(patchRow, patchCol);
			maxRoughness = std::max(maxRoughness, roughness);
			patchRoughness[count++] = roughness;
		}

	std::ofstream output(roughnessFilePath, std::ios::binary);

	for (unsigned i = 0; i < count; ++i)
		patchRoughness[i] /= maxRoughness;

	output.write((char*)patchRoughness, count * sizeof(float));
	output.close();
}


ID3D11ShaderResourceView* Terrain::loadInfomap(ID3D11Device* graphicsDevice, const D3DXVECTOR2* ranges, unsigned char numRanges) const
{
	ID3D11ShaderResourceView* infomap;

	// if infomap exists -> load it
	if (D3DX11CreateShaderResourceViewFromFileA(graphicsDevice, colorinfoFilePath, 0, 0, &infomap, 0) != D3D11_ERROR_FILE_NOT_FOUND)
		return infomap;

	unsigned numRows = heightmap.getNumRows();
	unsigned numCols = heightmap.getNumCols();

	unsigned char* info = new unsigned char[numRows * numCols * 4];

	unsigned counter = 0;

	for (unsigned row = numRows - 1; row < numRows; --row)
		for (unsigned col = 0; col < numCols; ++col)
		{
			float height = heightmap.getHeight(row, col);

			unsigned char index1 = 0;

			while (index1 < numRanges - 1 && height > ranges[index1 + 1].x)
				++index1;

			unsigned char index0 = index1 > 0 ? index1 - 1 : 0;

			info[counter++] = index0;
			info[counter++] = index1;
			info[counter++] = (unsigned char)(ranges[index1].x * 255);
			info[counter++] = (unsigned char)(ranges[index0].y * 255);
		}

	ID3D11Texture2D* texture;

	D3D11_TEXTURE2D_DESC desc;

	desc.Width = numCols;
	desc.Height = numRows;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;

	initData.pSysMem = info;
	initData.SysMemPitch = numCols * 4;
	initData.SysMemSlicePitch = numRows * numCols * 4;

	graphicsDevice->CreateTexture2D(&desc, &initData, &texture);

	delete[] info;

	ID3D11DeviceContext* immediateContext;
		
	graphicsDevice->GetImmediateContext(&immediateContext);

	D3DX11SaveTextureToFileA(immediateContext, texture, D3DX11_IFF_DDS, colorinfoFilePath);

	texture->Release();
	immediateContext->Release();

	D3DX11CreateShaderResourceViewFromFileA(graphicsDevice, colorinfoFilePath, 0, 0, &infomap, 0);
	
	return infomap;
}


ID3D11ShaderResourceView* Terrain::loadTextureArray(ID3D11Device* graphicsDevice, const char** textureNames, unsigned numTextures) const
{
	D3DX11_IMAGE_LOAD_INFO loadInfo;

	loadInfo.Width = D3DX11_DEFAULT;
	loadInfo.Height = D3DX11_DEFAULT;
	loadInfo.Depth = D3DX11_DEFAULT;
	loadInfo.FirstMipLevel = D3DX11_DEFAULT;
	loadInfo.MipLevels = D3DX11_DEFAULT;
	loadInfo.Usage = (D3D11_USAGE) D3D11_USAGE_STAGING;
	loadInfo.BindFlags = 0;
	loadInfo.CpuAccessFlags = D3D11_CPU_ACCESS_READ;
	loadInfo.MiscFlags = D3DX11_DEFAULT;
	loadInfo.Format = DXGI_FORMAT_FROM_FILE;
	loadInfo.Filter = D3DX11_DEFAULT;
	loadInfo.MipFilter = D3DX11_DEFAULT;
	loadInfo.pSrcInfo = NULL;

	ID3D11Texture2D** textures = new ID3D11Texture2D*[numTextures];

	for (unsigned i = 0; i < numTextures; i++)
	{
		ID3D11Resource* resource;

		D3DX11CreateTextureFromFileA(graphicsDevice, textureNames[i], &loadInfo, 0, &resource, 0);

		resource->QueryInterface(&textures[i]);
		resource->Release();
	}

	ID3D11ShaderResourceView* textureArray = createTextureArray(graphicsDevice, textures, numTextures);

	for (unsigned i = 0; i < numTextures; i++)
		textures[i]->Release();

	delete[] textures;

	return textureArray;
}


ID3D11ShaderResourceView* Terrain::createTextureArray(ID3D11Device* graphicsDevice, ID3D11Texture2D** textures, unsigned numTextures) const
{
	D3D11_TEXTURE2D_DESC textureDesc;
	ID3D11Texture2D* textureArray = 0;
	ID3D11ShaderResourceView* textureArrayView = 0;

	textures[0]->GetDesc(&textureDesc);

	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.ArraySize = numTextures;
		
	graphicsDevice->CreateTexture2D(&textureDesc, 0, &textureArray);

	ID3D11DeviceContext* immediateContext;

	graphicsDevice->GetImmediateContext(&immediateContext);

	for (unsigned i = 0; i < numTextures; ++i)
	{
		for (unsigned mip = 0; mip < textureDesc.MipLevels; mip++)
		{
			D3D11_MAPPED_SUBRESOURCE subresource;
			immediateContext->Map(textures[i], mip, D3D11_MAP_READ, 0, &subresource);
			immediateContext->UpdateSubresource(textureArray, D3D11CalcSubresource(mip, i, textureDesc.MipLevels), 0, subresource.pData, subresource.RowPitch, subresource.DepthPitch);
			immediateContext->Unmap(textures[i], mip);
		}
	}

	immediateContext->Release();

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;

	viewDesc.Format = textureDesc.Format;
	viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MipLevels = textureDesc.MipLevels;
	viewDesc.Texture2DArray.ArraySize = numTextures;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.FirstArraySlice = 0;

	graphicsDevice->CreateShaderResourceView(textureArray, &viewDesc, &textureArrayView);

	textureArray->Release();

	return textureArrayView;
}


void Terrain::initNode(Node* node, ID3D11Device* graphicsDevice, unsigned row, unsigned col, unsigned size, unsigned& numCurrentNodes, ID3D11Texture2D** textures, unsigned& numTextures, float* patchRoughness)
{
	node->textureIndex = numTextures++;

	unsigned numTotalPatchRows = numRows / patchSize;
	unsigned numTotalPatchCols = numCols / patchSize;

	unsigned numPatchRowsPerPatch = size / nodeSize;
	unsigned numPatchColsPerPatch = size / nodeSize;

	float* roughness = new float[numPatches];

	unsigned count = 0;

	for (unsigned patchRow = 0; patchRow < numPatchRows; ++patchRow)
		for (unsigned patchCol = 0; patchCol < numPatchCols; ++patchCol)
		{
			float currRoughness = 0;

			for (unsigned r = 0; r < numPatchRowsPerPatch; ++r)
				for (unsigned c = 0; c < numPatchColsPerPatch; ++c)
				{
					unsigned currPatchRow = row / patchSize + patchRow * numPatchRowsPerPatch + r;
					unsigned currPatchCol = col / patchSize + patchCol * numPatchColsPerPatch + c;

					unsigned patchIndex = getPatchIndex(currPatchRow, currPatchCol, numTotalPatchRows, numTotalPatchCols);

					currRoughness += patchRoughness[patchIndex];
				}

			roughness[count++] = currRoughness / (numPatchRowsPerPatch * numPatchColsPerPatch);
		}

	D3D11_TEXTURE2D_DESC desc;

	desc.Width = numPatchCols;
	desc.Height = numPatchRows;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;

	initData.pSysMem = roughness;
	initData.SysMemPitch = numPatchCols * sizeof(float);
	initData.SysMemSlicePitch = numPatches * sizeof(float);

	graphicsDevice->CreateTexture2D(&desc, &initData, &textures[node->textureIndex]);

	delete[] roughness;

	unsigned halfSize = size / 2;

	if (halfSize >= nodeSize)
	{
		for (unsigned r = 0; r < 2; ++r)
			for (unsigned c = 0; c < 2; ++c)
			{
				Node* child = &nodes[numCurrentNodes++];
				initNode(child, graphicsDevice, row + r * halfSize, col + c * halfSize, halfSize, numCurrentNodes, textures, numTextures, patchRoughness);
				node->children[r * 2 + c] = child;
			}
	}
}


void Terrain::updateNode(Node* node, unsigned row, unsigned col, unsigned size, unsigned& numInstances, const D3DXVECTOR3& cameraPos, const BoundingFrustum& viewFrustum, bool frustumCulling)
{
	if (frustumCulling)
	{
		Vector3 min = Vector3(col * terrainScale, 0.0f, row * terrainScale);
		Vector3 max = Vector3((col + size) * terrainScale, bumpiness, (row + size) * terrainScale);

		switch (viewFrustum.contains(BoundingBox(min, max)))
		{
			case ContainmentType::disjoint: return;
			case ContainmentType::contains: frustumCulling = false;
		}
	}
	
	unsigned halfSize = size / 2;

	unsigned centerCol = col + halfSize;
	unsigned centerRow = row + halfSize;

	if (halfSize >= nodeSize && (uniformPatchSizeEnabled || std::abs(cameraPos.x - (float)centerCol) < (float)size && std::abs(cameraPos.z - (float)centerRow) < (float)size))
	{
		for (unsigned r = 0; r < 2; ++r)
			for (unsigned c = 0; c < 2; ++c)
				updateNode(node->children[r * 2 + c], row + r * halfSize, col + c * halfSize, halfSize, numInstances, cameraPos, viewFrustum, frustumCulling);
	}
	else
	{
		Instance& instance = instances[numInstances++];

		instance.size = (float)size;
		instance.row = (float)row;
		instance.col = (float)col;
		instance.textureIndex = node->textureIndex;
	}
}


float Terrain::getPatchRoughness(unsigned patchRow, unsigned patchCol) const
{
	float roughness = 0.0f;
	float height[3][3];
	float avg[4];

	unsigned heightmapSize = std::max(heightmap.getNumRows(), heightmap.getNumCols());
	unsigned terrainSize = std::max(numRows, numCols);

	unsigned patchSize = (unsigned)(this->patchSize * ((float)heightmapSize / (float)terrainSize));

	unsigned row = patchRow * patchSize;
	unsigned col = patchCol * patchSize;

	unsigned maxRow = row + patchSize == heightmapSize ? 1 : 0;
	unsigned maxCol = col + patchSize == heightmapSize ? 1 : 0;
	unsigned minRow = row == 0 ? 1 : 0;
	unsigned minCol = col == 0 ? 1 : 0;

	for (unsigned r = row + minRow; r < row + patchSize - maxRow; ++r)
		for (unsigned c = col + minCol; c < col + patchSize - maxCol; ++c)
		{
			for (unsigned i = 0; i < 3; ++i)
				for (unsigned j = 0; j < 3; ++j)
					height[i][j] = heightmap.getHeight((r + i - 1), (c + j - 1));

			// straight
			avg[0] = height[1][0] + height[1][2];
			avg[1] = height[0][1] + height[2][1];

			// diagonal 
			avg[2] = height[0][0] + height[2][2];
			avg[3] = height[2][0] + height[0][2];

			for (unsigned i = 0; i < 4; ++i)
				roughness = std::max(roughness, std::abs(avg[i] * 0.5f - height[1][1]));
		}

	return roughness;
}


int Terrain::clamp(int value, int min, int max) const
{
	return value < min ? min : value > max ? max : value;
}


unsigned Terrain::getPatchIndex(unsigned patchRow, unsigned patchCol, unsigned numPatchRows, unsigned numPatchCols) const
{
	return patchRow * numPatchCols + patchCol;
}
