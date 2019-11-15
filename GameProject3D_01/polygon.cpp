
// インクルード ////////////////////////////////////
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "gameObject.h"
#include "polygon.h"

void CPolygon::Init(XMFLOAT2 position, XMFLOAT2 W_H, char* pFileName)
{
	VERTEX_3D vertex[4] = {

		vertex[0].Position = XMFLOAT3(position.x, position.y, 0.0f),	// "XM"が頭についてる変数・関数はDirectXMathライブラリの一部
		vertex[0].Normal   = XMFLOAT3(0.0f, 0.0f, -1.0f),
		vertex[0].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f),

		vertex[1].Position = XMFLOAT3(position.x + W_H.x, position.y, 0.0f),
		vertex[1].Normal   = XMFLOAT3(0.0f, 0.0f, -1.0f),
		vertex[1].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f),

		vertex[2].Position = XMFLOAT3(position.x, position.y + W_H.y, 0.0f),
		vertex[2].Normal   = XMFLOAT3(0.0f, 0.0f, -1.0f),
		vertex[2].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f),

		vertex[3].Position = XMFLOAT3(position.x + W_H.x, position.y + W_H.y, 0.0f),
		vertex[3].Normal   = XMFLOAT3(0.0f, 0.0f, -1.0f),
		vertex[3].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f),
	};

	// 頂点バッファ生成 //////
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;
	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBufer);

	// テクスチャ読み込み //////
	m_Texture = new CTexture();
	m_Texture->Load(pFileName);
}

void CPolygon::Uninit()
{
	m_VertexBufer->Release();
	delete m_Texture;
}


void CPolygon::Update()
{

}

void CPolygon::Draw()
{
	UINT Stride = sizeof(VERTEX_3D);
	UINT offdet = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBufer, &Stride, &offdet);
	CRenderer::SetTexture(m_Texture);
	CRenderer::SetWorldViewProjection2D();
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CRenderer::GetDeviceContext()->Draw(4, 0);
}
