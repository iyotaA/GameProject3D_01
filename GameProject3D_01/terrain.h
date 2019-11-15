#ifndef _TERRAIN_H_
#define _TERRAIN_H_

////////////////////////////////////////////////////////////////////////////////
// Class name: TerrainClass
////////////////////////////////////////////////////////////////////////////////
class CTerrain : public CGameObject
{
public:

	void Init();
	void Uninit();
	void Update();
	void Draw();

	int GetIndexCount();
	float GetHeight(XMFLOAT3* _position);

private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	struct HeightMapType
	{
		float x, y, z;
	};

	bool LoadHeightMap(char*);
	void NormalizeHeightMap();
	void UnloadHeightMap();

	bool InitializeBuffers();
	void UninitBuffers();
	void DrawBuffers();
	void DrawGUI();

	int m_terrainWidth, m_terrainHeight;
	int m_vertexCount, m_indexCount;
	ID3D11Buffer* m_indexBuffer = nullptr;
	ID3D11Buffer* m_vertexBuffer = nullptr;

	HeightMapType* m_heightMap = nullptr;
	VERTEX_3D* m_Vertex;
	CTexture* m_Texture;

	// デバッグ用
	XMFLOAT3 PlayerArea = XMFLOAT3(0.0f, 0.0f, 0.0f);
	int PlayerArea_Index_W = 0;
	int PlayerArea_Index_H = 0;
	bool IsRange = false;
};

#endif