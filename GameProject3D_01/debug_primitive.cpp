//=================================================================================
// インクルード
//=================================================================================
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "debug_primitive.h"
#include "imGui_all.h"

//=================================================================================
// マクロ定義
//=================================================================================
#define CIRCLE_VERTEX_COUNT	(8)
#define CIRCLE_DRAW_MAX		(2048)


ID3D11Buffer*	CDebugPrimitive::m_VertexBufer;
ID3D11Buffer*	CDebugPrimitive::m_IndexBufer;
VERTEX_3D*		CDebugPrimitive::m_pVertex;
WORD*           CDebugPrimitive::m_pIndex;
int             CDebugPrimitive::m_CircleCount;
bool			CDebugPrimitive::m_IsDisplayed;


void CDebugPrimitive::DebugPrimitive_Init(void)
{
	m_CircleCount = 0;
	m_IsDisplayed = false;

	m_pVertex = new VERTEX_3D[CIRCLE_VERTEX_COUNT * CIRCLE_DRAW_MAX];
	m_pIndex  = new WORD[CIRCLE_VERTEX_COUNT * 2 * CIRCLE_DRAW_MAX];

	//// 頂点バッファ確保
	//DirectX3D_GetDevice()->CreateVertexBuffer(sizeof(DebugVertex) * CIRCLE_VERTEX_COUNT * CIRCLE_DRAW_MAX, D3DUSAGE_WRITEONLY, FVF_DEBUG_VERTEX, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);
	//// インデックスバッファ確保
	//DirectX3D_GetDevice()->CreateIndexBuffer(sizeof(WORD) * CIRCLE_VERTEX_COUNT * 2 * CIRCLE_DRAW_MAX, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL);
}

void CDebugPrimitive::DebugPrimitive_Uninit(void)
{
	if(m_VertexBufer)	m_VertexBufer->Release();
	if(m_IndexBufer)	m_IndexBufer->Release();

	delete m_pVertex;
	delete m_pIndex;

	//SAFE_RELEASE(g_pIndexBuffer);
	//SAFE_RELEASE(g_pVertexBuffer);
}

void CDebugPrimitive::DebugPrimitive_BatchBegin(void)
{
	// サークル描画数をゼロに
	m_CircleCount = 0;

	//g_pVertexBuffer->Lock(0, 0, (void**)&g_pVertex, 0);
	//g_pIndexBuffer->Lock(0, 0, (void**)&g_pIndex, 0);
}

void CDebugPrimitive::DebugPrimitive_BatchCirecleDraw(const XMFLOAT3* pos, float radius, const XMFLOAT4* color)
{
	//表示する時のみ
	if (!m_IsDisplayed)return;


	int n = m_CircleCount * CIRCLE_VERTEX_COUNT;
	float a = PI * 2.0f / CIRCLE_VERTEX_COUNT;

	// ( 円 1 ) //////////////////////////////////////////////////////////////
	n = m_CircleCount * CIRCLE_VERTEX_COUNT;

	for (int i = 0; i < CIRCLE_VERTEX_COUNT; i++) {
		m_pVertex[n + i].Position.x = cos(a * i) * radius + pos->x;
		m_pVertex[n + i].Position.y = sin(a * i) * radius + pos->y;
		m_pVertex[n + i].Position.z = pos->z;
		m_pVertex[n + i].Normal     = XMFLOAT3(0.0f, 1.0f, 0.0f);
		m_pVertex[n + i].Diffuse    = *color;
		m_pVertex[n + i].TexCoord   = XMFLOAT2(0.0f, 0.0f);
	}

	for (int i = 0; i < CIRCLE_VERTEX_COUNT; i++) {
		m_pIndex[n * 2 + i * 2] = n + i;
		m_pIndex[n * 2 + i * 2 + 1] = n + (i + 1) % CIRCLE_VERTEX_COUNT;
	}
	m_CircleCount++;

	// ( 円 2 ) //////////////////////////////////////////////////////////////
	n = m_CircleCount * CIRCLE_VERTEX_COUNT;

	for (int i = 0; i < CIRCLE_VERTEX_COUNT; i++) {
		m_pVertex[n + i].Position.x = pos->x;
		m_pVertex[n + i].Position.y = cos(a * i) * radius + pos->y;
		m_pVertex[n + i].Position.z = sin(a * i) * radius + pos->z;
		m_pVertex[n + i].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		m_pVertex[n + i].Diffuse = *color;
		m_pVertex[n + i].TexCoord = XMFLOAT2(0.0f, 0.0f);
	}


	for (int i = 0; i < CIRCLE_VERTEX_COUNT; i++) {
		m_pIndex[n * 2 + i * 2] = n + i;
		m_pIndex[n * 2 + i * 2 + 1] = n + (i + 1) % CIRCLE_VERTEX_COUNT;
	}

	m_CircleCount++;

	// ( 円 3 ) //////////////////////////////////////////////////////////////
	n = m_CircleCount * CIRCLE_VERTEX_COUNT;

	for (int i = 0; i < CIRCLE_VERTEX_COUNT; i++) {
		m_pVertex[n + i].Position.x = cos(a * i) * radius + pos->x;
		m_pVertex[n + i].Position.y = pos->y;
		m_pVertex[n + i].Position.z = sin(a * i) * radius + pos->z;
		m_pVertex[n + i].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		m_pVertex[n + i].Diffuse = *color;
		m_pVertex[n + i].TexCoord = XMFLOAT2(0.0f, 0.0f);
	}

	for (int i = 0; i < CIRCLE_VERTEX_COUNT; i++) {
		m_pIndex[n * 2 + i * 2] = n + i;
		m_pIndex[n * 2 + i * 2 + 1] = n + (i + 1) % CIRCLE_VERTEX_COUNT;
	}

	m_CircleCount++;
}

void CDebugPrimitive::DebugPrimitive_BatchRun(void)
{
	//ImGui Render
	DrawGUI();

	//表示する時のみ
	if (!m_IsDisplayed)return;

	// ヴァーテクスバッファ生成 //////
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * m_CircleCount * CIRCLE_VERTEX_COUNT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_pVertex;
		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBufer);
	}

	// インデックスバッファ生成 //////
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD) * CIRCLE_VERTEX_COUNT * 2 * m_CircleCount;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_pIndex;
		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBufer);
	}

	XMMATRIX world;
	world = XMMatrixIdentity();
	CRenderer::SetWorldMatrix(&world);

	// ライト初期化
	//LIGHT light;
	//light.Direction = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	//light.Diffuse = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//light.Ambient = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//CRenderer::SetLight(light);

	UINT Stride = sizeof(VERTEX_3D);
	UINT offdet = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBufer, &Stride, &offdet);	// バーテクスバッファセット
	CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBufer, DXGI_FORMAT_R16_UINT, 0);		// インデックスバッファセット
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	CRenderer::GetDeviceContext()->DrawIndexed(CIRCLE_VERTEX_COUNT * 2 * m_CircleCount, 0, 0);


	//g_pVertexBuffer->Unlock();
	//g_pIndexBuffer->Unlock();

	//LPDIRECT3DDEVICE9 pDevice = DirectX3D_GetDevice();

	//// 一番手前に描画
	//pDevice->Clear(0, NULL, D3DCLEAR_ZBUFFER, NULL, 1.0f, 0);

	//D3DXMATRIX MtxWorld;
	//D3DXMatrixIdentity(&MtxWorld);

	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//pDevice->SetTransform(D3DTS_WORLD, &MtxWorld);	// ワールド座標返還用の行列をデバイスにセット
	//pDevice->SetFVF(FVF_DEBUG_VERTEX);
	//pDevice->SetTexture(0, NULL);

	//pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(DebugVertex));
	//pDevice->SetIndices(g_pIndexBuffer);

	//pDevice->DrawIndexedPrimitive(D3DPT_LINELIST, 0, 0, g_CircleCount * CIRCLE_VERTEX_COUNT, 0, g_CircleCount * CIRCLE_VERTEX_COUNT);
}

void CDebugPrimitive::DrawGUI()
{
	using namespace ImGui;

	Begin("Collision");
	{
		Checkbox("DisplayCollision", &m_IsDisplayed);
	}
	End();
}
