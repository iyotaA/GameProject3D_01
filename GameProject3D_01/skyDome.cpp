
// インクルード ////////////////////////////////////
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "gameObject.h"
#include "shader_all.h"
#include "skyDome.h"

#include "camera_manager.h"
#include "camera.h"


#define Radius		(400.0f)
#define Height		(300.0f)
#define GridNumX	(500)
#define GridNumZ	(60)
#define RotateSpeed (0.005f)

void CSkyDome::Init()
{
	/* ヴァーテクス数計測 */
	int VertexNum = (GridNumX + 1) * (GridNumZ + 1);

	/* インデックス数計測 */
	m_IndexNum = (2 + GridNumX * 2) * (GridNumZ + 1) + GridNumZ * 2;

	// ヴァーテクス情報格納
	{
		VERTEX_3D* pVertex;
		pVertex = new VERTEX_3D[ VertexNum];

		for (int z = 0; z < GridNumZ + 1; z++) {
			for (int x = 0; x < GridNumX + 1; x++) {
				float radius;			// ドームの半径（徐々に小さくなる）

				if (z == GridNumZ) {	// 最後の列の頂点
					radius = 0.0f;
				}
				else {                  // 最後以外の列
					radius = sinf(((180.0f / GridNumZ) * z) * PI / 180.0f) * Radius;
				}


				float height = sinf((270.0f + (180.0f  / GridNumZ) * z) * PI / 180.0f) * Height;
				float Pos_x  = cosf(((360.0f / GridNumX) * x) * PI / 180.0f) * radius;
				float Pos_z  = sinf(((360.0f / GridNumX) * x) * PI / 180.0f) * radius;

				if (x == 0) {
					pVertex[x + (GridNumX + 1) * z] = {
						Vector3(Pos_x, height, Pos_z),
						Vector3(0.0f, 1.0f, 0.0f),
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
						XMFLOAT2((float)x / GridNumX + 0.01f, (float)(GridNumZ - z) / GridNumZ)
					};
				}
				else if (x == GridNumX) {
					pVertex[x + (GridNumX + 1) * z] = {
						Vector3(Pos_x, height, Pos_z),
						Vector3(0.0f, 1.0f, 0.0f),
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
						XMFLOAT2((float)x / GridNumX - 0.01f, (float)(GridNumZ - z) / GridNumZ)
					};
				}
				else {
					pVertex[x + (GridNumX + 1) * z] = {
						Vector3(Pos_x, height, Pos_z),
						Vector3(0.0f, 1.0f, 0.0f),
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
						XMFLOAT2((float)x / GridNumX, (float)(GridNumZ - z) / GridNumZ)
					};
				}
			}
		}
		// ヴァーテクスバッファ生成 //////
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = pVertex;
		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBufer);
	}

	// インデックス情報格納
	{
		WORD* pIndex;
		pIndex = new WORD[m_IndexNum];

		for (int z = 0, i = 0; z < GridNumZ + 1; z++) {
			for (int x = 0; x < GridNumX + 1; x++) {

				if (x == GridNumX) {/* 右端の頂点情報（行の終わり） */
					pIndex[i] = x + (GridNumX + 1) * z + (GridNumX + 1);
					i++;
					pIndex[i] = x + (GridNumX + 1) * z;
					i++;
					if (i < m_IndexNum) {/* 一番最後のインデックスが格納されていれば縮退ポリゴンは作らない */
						pIndex[i] = x + (GridNumX + 1) * z;
						i++;
					}
				}
				else if (z && !x) {	/* 左端の頂点情報（行の始め） */
					pIndex[i] = x + (GridNumX + 1) * z + (GridNumX + 1);
					i++;
					pIndex[i] = x + (GridNumX + 1) * z + (GridNumX + 1);
					i++;
					pIndex[i] = x + (GridNumX + 1) * z;
					i++;
				}
				else {				/* それ以外の頂点情報 */
					pIndex[i] = x + (GridNumX + 1) * z + (GridNumX + 1);
					i++;
					pIndex[i] = x + (GridNumX + 1) * z;
					i++;
				}
			}
		}
		// インデックスバッファ生成 //////
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD) * m_IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = pIndex;
		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBufer);
	}

	// テクスチャ読み込み /////
	m_Texture = new CTexture();
	m_Texture->LoadSTB("asset/image/skydome.png");

	// シェーダー読み込み //////
	m_Shader = ShaderManager::GetShader<CShaderDefault>();

	// トランスフォーム初期化
	m_Position = Vector3(0.0f, 0.0f, 0.0f);
	m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
	m_Scale = Vector3(1.0f, 1.0f, 1.0f);
}

void CSkyDome::Uninit()
{
	m_VertexBufer->Release();
	m_IndexBufer->Release();
	delete m_Texture;
}

void CSkyDome::Update()
{
	m_Rotation.y += XMConvertToRadians(RotateSpeed);
}

void CSkyDome::Draw()
{
	XMMATRIX world;
	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	world *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	XMFLOAT4X4 world_4x4;
	XMStoreFloat4x4(&world_4x4, world);

	CCamera* camera = CCameraManager::GetCamera();

	m_Shader->SetWorldMatrix(&world_4x4);
	m_Shader->SetViewMatrix(&camera->GetViewMatrix());
	m_Shader->SetProjectionMatrix(&camera->GetProjectionMatrix());
	m_Shader->SetLight(LIGHT());

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = COLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_Shader->SetMaterial(material);
	m_Shader->Set();

	UINT Stride = sizeof(VERTEX_3D);
	UINT offdet = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBufer, &Stride, &offdet);	// バーテクスバッファセット
	CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBufer, DXGI_FORMAT_R16_UINT, 0);		// インデックスバッファセット
	CRenderer::SetTexture(m_Texture);
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CRenderer::GetDeviceContext()->DrawIndexed(m_IndexNum, 0, 0);
}
