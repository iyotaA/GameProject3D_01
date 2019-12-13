
#include "main.h"
#include "imGui_all.h"
#include "renderer.h"
#include "texture.h"
#include "gameObject.h"
#include "shader_all.h"
#include "terrain.h"
#include "camera_manager.h"
#include "camera.h"

#define GRID_SIZE 1.5f


void CTerrain::Init()
{
	bool result;

	result = LoadHeightMap("asset/image/height_map000.bmp");
	assert(result);

	// テクスチャ読み込み //////
	m_Texture = new CTexture*[m_TextureNum];
	m_Texture[0] = new CTexture();
	m_Texture[1] = new CTexture();
	m_Texture[2] = new CTexture();

	// シェーダー読み込み //////
	m_Shader = ShaderManager::GetShader<CShaderNormalMap>();

	m_Texture[0]->LoadSTB("asset/image/field_dart001.png");
	m_Texture[1]->LoadSTB("asset/image/field_grass001.png");
	m_Texture[2]->LoadSTB("asset/image/NormalMap_dart.png");

	result = InitializeBuffers();
	assert(result);
}


void CTerrain::Uninit()
{
	// Release the vertex and index buffer.
	m_vertexBuffer->Release();
	m_indexBuffer->Release();

	UnloadHeightMap();

	return;
}

void CTerrain::Update()
{

}

void CTerrain::Draw()
{
	XMMATRIX world = XMMatrixIdentity();

	XMFLOAT4X4 world_4x4;
	XMFLOAT4X4 mtxWIT4x4;
	XMMATRIX mtxWIT;

	// ワールド行列変換
	XMStoreFloat4x4(&world_4x4, world);

	// World * Inverse * Transpose
	mtxWIT = XMMatrixInverse(nullptr, world);
	mtxWIT = XMMatrixTranspose(mtxWIT);
	XMStoreFloat4x4(&mtxWIT4x4, mtxWIT);

	m_Shader->SetMtxWIT(&mtxWIT4x4);

	CCamera* camera = CCameraManager::GetCamera();

	m_Shader->SetWorldMatrix(&world_4x4);
	m_Shader->SetViewMatrix(&camera->GetViewMatrix());
	m_Shader->SetProjectionMatrix(&camera->GetProjectionMatrix());
	XMFLOAT4 camera_pos = XMFLOAT4(camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z, 1.0f);
	m_Shader->SetCameraPosition(&camera_pos);
	m_Shader->SetLight(LIGHT());

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = COLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_Shader->SetMaterial(material);
	m_Shader->Set();

	DrawBuffers();
	DrawGUI();
}


int CTerrain::GetIndexCount()
{
	return m_indexCount;
}

bool CTerrain::LoadHeightMap(char* filename)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize, k, index;
	unsigned char* bitmapImage;
	unsigned char height;

	error = fopen_s(&filePtr, filename, "rb");
	assert(error == 0);
	assert(filePtr != 0);

	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	assert(count == 1);

	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	assert(count == 1);
	assert(filePtr);

	m_terrainWidth = bitmapInfoHeader.biWidth;
	m_terrainHeight = bitmapInfoHeader.biHeight;

	imageSize = m_terrainWidth * m_terrainHeight * 3;

	bitmapImage = new unsigned char[imageSize];
	assert(bitmapImage);

	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	count = fread(bitmapImage, 1, imageSize, filePtr);
	assert(count == imageSize);
	assert(filePtr);

	error = fclose(filePtr);
	assert(error == 0);

	m_heightMap = new HeightMapType[m_terrainWidth * m_terrainHeight];
	assert(m_heightMap);

	k = 0;

	float offset_x = GRID_SIZE * m_terrainWidth / 2.0f;
	float offset_z = GRID_SIZE * m_terrainHeight / 2.0f;

	for (int z = 0; z < m_terrainHeight; z++)
	{
		for (int x = 0; x < m_terrainWidth; x++)
		{
			height = bitmapImage[k];

			index = (m_terrainHeight * z) + x;

			m_heightMap[index].x = (float)x * GRID_SIZE - offset_x;
			m_heightMap[index].y = (float)height / 15.0f * GRID_SIZE; // NormalizeHeight
			m_heightMap[index].z = (float)z * GRID_SIZE - offset_z;

			k += 3;
		}
	}

	delete[] bitmapImage;
	bitmapImage = 0;

	return true;
}

void CTerrain::UnloadHeightMap()
{
	SAFE_DELETE(m_heightMap)
}

bool CTerrain::InitializeBuffers()
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	/* ヴァーテクス数計測 */
	m_vertexCount = m_terrainWidth * m_terrainHeight;

	/* インデックス数計測 */
	m_indexCount = (2 + (m_terrainWidth - 1) * 2) * (m_terrainHeight - 1) + (m_terrainHeight - 2) * 2;


	// ヴァーテクス情報格納
	{
		m_Vertex = new VERTEX_3D_NOMAL_MAP[m_vertexCount];

		for (int z = 0; z < m_terrainHeight; z++) {
			for (int x = 0; x < m_terrainWidth; x++) {
				float V;
				/* V座標（UV）設定 */
				if (z % 2 == 0) {
					V = 0.0f;
				}
				else {
					V = 1.0f;
				}

				int blend = ((int)m_heightMap[m_terrainHeight * z + x].y <= 0) ? 1 :(int)m_heightMap[m_terrainHeight * z + x].y;

				m_Vertex[m_terrainHeight * z + x] = {

					Vector3(m_heightMap[m_terrainHeight * z + x].x, m_heightMap[m_terrainHeight * z + x].y, m_heightMap[m_terrainHeight * z + x].z),
					Vector3(0.0f, 0.0f, 1.0f),
					Vector3(1.0f, 0.0f, 0.0f),
					Vector3(0.0f, 1.0f, 0.0f),
					XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
					XMFLOAT2(x, z),
					max((rand() % blend) * (1.0f / blend), m_heightMap[m_terrainHeight * z + x].y / (10.0f * GRID_SIZE))
				};
			}
		}

		// 法線計算
		for (int z = 1; z < m_terrainHeight - 1; z++) {
			for (int x = 1; x < m_terrainWidth - 1; x++) {

				Vector3 va, vb, vc;
				va.x = m_Vertex[x + 1 + m_terrainWidth * z].Position.x - m_Vertex[x - 1 + m_terrainWidth * z].Position.x;
				va.y = m_Vertex[x + 1 + m_terrainWidth * z].Position.y - m_Vertex[x - 1 + m_terrainWidth * z].Position.y;
				va.z = m_Vertex[x + 1 + m_terrainWidth * z].Position.z - m_Vertex[x - 1 + m_terrainWidth * z].Position.z;
				vb.x = m_Vertex[x + m_terrainWidth + m_terrainWidth * z].Position.x - m_Vertex[x - m_terrainWidth + m_terrainWidth * z].Position.x;
				vb.y = m_Vertex[x + m_terrainWidth + m_terrainWidth * z].Position.y - m_Vertex[x - m_terrainWidth + m_terrainWidth * z].Position.y;
				vb.z = m_Vertex[x + m_terrainWidth + m_terrainWidth * z].Position.z - m_Vertex[x - m_terrainWidth + m_terrainWidth * z].Position.z;

				vc.x = va.y * vb.z - va.z * vb.y;
				vc.y = va.z * vb.x - va.x * vb.z;
				vc.z = va.x * vb.y - va.y * vb.x;

				float length;
				length = sqrtf(vc.x * vc.x + vc.y * vc.y + vc.z * vc.z);	// （三平方の定理）長さを求める

				// ベクトルの正規化
				vc.x = vc.x / length;
				vc.y = vc.y / length;
				vc.z = vc.z / length;

				m_Vertex[x + m_terrainWidth * z].Normal = vc;
			}
		}
	}

	// インデックス情報格納
	UINT* indices;
	indices = new UINT[m_indexCount];
	{
		/* インデックス情報格納 */
		for (int z = 0, i = 0; z < m_terrainHeight - 1; z++) {
			for (int x = 0; x < m_terrainWidth; x++) {
				if (x == (m_terrainWidth - 1)) {/* 右端の頂点情報（行の終わり） */
					indices[i] = x + m_terrainWidth * z + m_terrainWidth;
					i++;
					indices[i] = x + m_terrainWidth * z;
					i++;
					if (i < m_indexCount) {/* 一番最後のインデックスが格納されていれば縮退ポリゴンは作らない */
						indices[i] = x + m_terrainWidth * z;
						i++;
					}
				}
				else if (z && (x == 0)) {	/* 左端の頂点情報（行の始め） */
					indices[i] = x + m_terrainWidth * z + m_terrainWidth;
					i++;
					indices[i] = x + m_terrainWidth * z + m_terrainWidth;
					i++;
					indices[i] = x + m_terrainWidth * z;
					i++;
				}
				else {				/* それ以外の頂点情報 */
					indices[i] = x + m_terrainWidth * z + m_terrainWidth;
					i++;
					indices[i] = x + m_terrainWidth * z;
					i++;
				}
			}
		}
	}

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VERTEX_3D_NOMAL_MAP) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = m_Vertex;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = CRenderer::GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	assert(SUCCEEDED(result));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned int) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = CRenderer::GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	assert(SUCCEEDED(result));

	delete[] indices;
	indices = 0;

	return true;
}


void CTerrain::UninitBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void CTerrain::DrawBuffers()
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VERTEX_3D_NOMAL_MAP);
	offset = 0;

	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	CRenderer::GetDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	CRenderer::SetTexture(m_Texture, 0, m_TextureNum);

	CRenderer::SetRasterizerState(D3D11_FILL_SOLID, D3D11_CULL_FRONT);
	CRenderer::DrawIndexed(m_indexCount, 0, 0);
	CRenderer::SetRasterizerState(D3D11_FILL_SOLID, D3D11_CULL_NONE);
}

void CTerrain::DrawGUI()
{
	ImGui::Begin("System");

	if (ImGui::CollapsingHeader("Field"))
	{
		ImGui::Columns(2, "Field");

		if (ImGui::CollapsingHeader("PlayerAreaPosition"))
		{
			int id = ImGui::GetColumnIndex();
			float width = ImGui::GetColumnWidth(id);

			ImGuiID HeaderId = ImGui::GetID("PlayerAreaParam");

			ImGui::BeginChildFrame(HeaderId, ImVec2(width, 60));
			ImGui::Text("StandArea_x = %.1f", PlayerArea.x);
			ImGui::Text("StandArea_y = %.1f", PlayerArea.y);
			ImGui::Text("StandArea_z = %.1f", PlayerArea.z);
			ImGui::EndChildFrame();
		}

		ImGui::NextColumn();

		//＜:: Animation ::＞
		if (ImGui::CollapsingHeader("PlayerAreaStatus"))
		{
			int id = ImGui::GetColumnIndex();
			float width = ImGui::GetColumnWidth(id);

			ImGuiID HeaderId = ImGui::GetID("PlayerAreaStatus");

			ImGui::BeginChildFrame(HeaderId, ImVec2(width, 60));

			ImGui::Text("index_Width = %d", PlayerArea_Index_W);
			ImGui::Text("index_Height = %d", PlayerArea_Index_H);
			ImGui::Text(IsRange ? "Range" : "Out of Range");

			ImGui::EndChildFrame();
		}

	}

	ImGui::End();
}

float CTerrain::GetHeight(XMFLOAT3* _position)
{
	XMFLOAT3 position = *_position;

	if (!((position.x >= m_Vertex[0].Position.x && position.x <= m_Vertex[m_terrainWidth * m_terrainHeight - 1].Position.x) &&
		(position.z >= m_Vertex[0].Position.z && position.z <= m_Vertex[m_terrainWidth * m_terrainHeight - 1].Position.z)))
	{
		IsRange = false;
		return FAILD_NUM;
	}

	int x, z;
	XMFLOAT3 p0, p1, p2, v01, v02, n, v, hp, va, vb;
	float dp0n, dvn, dpn, t;

	// プレイヤーから伸びる下方向のベクトル
	v = XMFLOAT3(0.0f, -1.0f, 0.0f);

	float offset_x =  GRID_SIZE * m_terrainWidth / 2.0f;
	float offset_z =  GRID_SIZE * m_terrainHeight / 2.0f;

	x = static_cast<int>(position.x + offset_x) / GRID_SIZE;
	z = static_cast<int>(position.z + offset_z) / GRID_SIZE;

	PlayerArea_Index_W = x;
	PlayerArea_Index_H = z;
	// va : 四角ポリゴン（三角*2）の対角線のベクトル
	// vb : 四角ポリゴン（三角*2）の左上座標からプレイヤーの位置までのベクトル
	va.x = m_Vertex[(x + 1) + m_terrainWidth * (z + 1)].Position.x - m_Vertex[x + m_terrainWidth * z].Position.x;
	va.y = m_Vertex[(x + 1) + m_terrainWidth * (z + 1)].Position.y - m_Vertex[x + m_terrainWidth * z].Position.y;
	va.z = m_Vertex[(x + 1) + m_terrainWidth * (z + 1)].Position.z - m_Vertex[x + m_terrainWidth * z].Position.z;
	vb.x = position.x - m_Vertex[x + m_terrainWidth * z].Position.x;
	vb.y = position.y - m_Vertex[x + m_terrainWidth * z].Position.y;
	vb.z = position.z - m_Vertex[x + m_terrainWidth * z].Position.z;

	// va × vb の y 成分 > 0.0f
	if ((va.z * vb.x - va.x * vb.z > 0.0f) || (va.z * vb.x - va.x * vb.z == 0.0f)) {

		p0 = m_Vertex[x + m_terrainWidth * (z + 1)].Position;		// p1|＼
		p1 = m_Vertex[x + m_terrainWidth * z].Position;				//   |　＼
		p2 = m_Vertex[(x + 1) + m_terrainWidth * (z + 1)].Position; // p0|____＼p2
	}
	// va × vb の y 成分 < 0.0f
	else {
		p0 = m_Vertex[(x + 1) + m_terrainWidth * z].Position;       // p2＼'''''|p0
		p1 = m_Vertex[(x + 1) + m_terrainWidth * (z + 1)].Position; //     ＼   |
		p2 = m_Vertex[x + m_terrainWidth * z].Position;             //   　   ＼|p1
	}

	// p1 - p0
	v01 = XMFLOAT3(p1.x - p0.x, p1.y - p0.y, p1.z - p0.z);

	// p2 - p0
	v02 = XMFLOAT3(p2.x - p0.x, p2.y - p0.y, p2.z - p0.z);

	// v01 ×　v02
	n = XMFLOAT3(v01.y * v02.z - v01.z * v02.y, v01.z * v02.x - v01.x * v02.z, v01.x * v02.y - v01.y * v02.x);

	// v ・ n
	dvn = v.x * n.x + v.y * n.y + v.z * n.z;

	// p0 ・ n
	dp0n = p0.x * n.x + p0.y * n.y + p0.z * n.z;

	// position・n
	dpn = position.x * n.x + position.y * n.y + position.z * n.z;

	t = (dp0n - dpn) / dvn;

	// position + v * t
	hp = XMFLOAT3(position.x + v.x * t, position.y + v.y * t, position.z + v.z * t);
	//hp.y = position.y + v.y * t;

	PlayerArea = XMFLOAT3(hp.x, hp.y, hp.z);
	IsRange = true;

	return hp.y;
}

