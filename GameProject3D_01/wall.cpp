
// インクルード ////////////////////////////////////
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "gameObject.h"
#include "wall.h"

#define SIZE		(1.0f)
#define GridNumX	(30)
#define GridNumZ	(5)

void CWall::Init()
{
	/* ヴァーテクス数計測 */
	int VertexNum = (GridNumX + 1) * (GridNumZ + 1);

	/* インデックス数計測 */
	m_IndexNum = (2 + GridNumX * 2) * GridNumZ + (GridNumZ - 1) * 2;

	float ShiftedAmount_x = (float)SIZE * GridNumX / 2;	// メッシュフィールドの中心を世界の中心にするためにずらす量
	float ShiftedAmount_z = (float)SIZE * GridNumZ / 2;	// メッシュフィールドの中心を世界の中心にするためにずらす量

	// ヴァーテクス情報格納
	{
		VERTEX_3D* pVertex;
		pVertex = new VERTEX_3D[VertexNum];

		for (int z = 0; z < GridNumZ + 1; z++) {
			for (int x = 0; x < GridNumX + 1; x++) {
				float V;
				/* V座標（UV）設定 */
				if (z % 2 == 0) {
					V = 0.0f;
				}
				else {
					V = 1.0f;
				}

				pVertex[x + (GridNumX + 1) * z] = {
					Vector3(x * SIZE - ShiftedAmount_x, 0.0f, -z * SIZE + ShiftedAmount_z),
					Vector3(0.0f, 1.0f, 0.0f),
					XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
					XMFLOAT2(x, z)
				};
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
		/* インデックス情報格納 */
		for (int z = 0, i = 0; z < GridNumZ; z++) {
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

	// テクスチャ読み込み //////
	m_Texture = new CTexture();
	m_Texture->LoadSTB("asset/image/dart000.png");

	// トランスフォーム初期化
	m_Position = Vector3(0.0f, 0.0f, 0.0f);
	m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
	m_Scale = Vector3(1.0f, 1.0f, 1.0f);
}

void CWall::Uninit()
{
	m_VertexBufer->Release();
	m_IndexBufer->Release();
	delete m_Texture;
}

void CWall::Update(const Vector3 pos)
{

}

void CWall::Draw()
{
	XMMATRIX world;
	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	world *= XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_Rotation.x), XMConvertToRadians(m_Rotation.y), XMConvertToRadians(m_Rotation.z));
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	//CRenderer::SetWorldMatrix(&world);

	UINT Stride = sizeof(VERTEX_3D);
	UINT offdet = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBufer, &Stride, &offdet);	// バーテクスバッファセット
	CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBufer, DXGI_FORMAT_R16_UINT, 0);		// インデックスバッファセット
	CRenderer::SetTexture(m_Texture);
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CRenderer::GetDeviceContext()->DrawIndexed(m_IndexNum, 0, 0);
}

