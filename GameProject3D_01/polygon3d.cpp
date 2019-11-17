
// インクルード ////////////////////////////////////
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "gameObject.h"
#include "field.h"
#include "polygon3d.h"


void CPolygon3D::Init(Vector3 pos, Vector3 scal, Vector3 rot)
{
	VERTEX_3D vertex[4] = {

		vertex[0].Position = Vector3(-0.5f, 0.0f, 0.5f),	// "XM"が頭についてる変数・関数はDirectXMathライブラリの一部
		vertex[0].Normal   = Vector3(0.0f, 1.0f, 0.0f),
		vertex[0].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f),

		vertex[1].Position = Vector3(0.5f, 0.0f, 0.5f),
		vertex[1].Normal   = Vector3(1.0f, 1.0f, 0.0f),
		vertex[1].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f),

		vertex[2].Position = Vector3(-0.5f, 0.0f, -0.5f),
		vertex[2].Normal   = Vector3(0.0f, 1.0f, 0.0f),
		vertex[2].Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f),

		vertex[3].Position = Vector3(0.5f, 0.0f, -0.5f),
		vertex[3].Normal   = Vector3(0.0f, 1.0f, 0.0f),
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

	////////////////////////////////////////////////////////////////////////////////////////


	// テクスチャ読み込み //////
	m_Texture = new CTexture();
	m_Texture->LoadSTB("asset/field001.tga");

	// トランスフォーム初期化
	m_Position = pos;
	m_Rotation.x = XMConvertToRadians(rot.x);
	m_Rotation.y = XMConvertToRadians(rot.y);
	m_Rotation.z = XMConvertToRadians(rot.z);
	m_Scale = scal;

}

void CPolygon3D::Uninit()
{
	m_VertexBufer->Release();
	delete m_Texture;
}

void CPolygon3D::Update()
{
	m_Position.x = cosf(XMConvertToRadians(m_Counter)) * 5;
	m_Position.z = sinf(XMConvertToRadians(m_Counter) * 2) * 5;

	m_Counter += 1;
}

void CPolygon3D::Draw()
{
	UINT Stride = sizeof(VERTEX_3D);
	UINT offdet = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBufer, &Stride, &offdet);
	CRenderer::SetTexture(m_Texture);

	XMMATRIX world;
	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	world *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	CRenderer::SetWorldMatrix(&world);

	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CRenderer::GetDeviceContext()->Draw(4, 0);
}
