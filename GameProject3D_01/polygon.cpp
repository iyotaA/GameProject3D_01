
// インクルード ////////////////////////////////////
#include "main.h"
#include "shader_all.h"
#include "renderer.h"
#include "texture.h"
#include "gameObject.h"
#include "polygon.h"
#include <regex>
#include <iostream>
void CPolygon2D::Init(char* pFileName)
{
	VERTEX_2D vertices[4] = {

		Vector3(m_Position.x, m_Position.y, 0.0f),
		m_Color,
		XMFLOAT2(m_UV.su, m_UV.sv),

		Vector3(m_Position.x + m_Size.x, m_Position.y, 0.0f),
		m_Color,
		XMFLOAT2(m_UV.eu, m_UV.sv),

		Vector3(m_Position.x, m_Position.y + m_Size.y, 0.0f),
		m_Color,
		XMFLOAT2(m_UV.su, m_UV.ev),

		Vector3(m_Position.x + m_Size.x, m_Position.y + m_Size.y, 0.0f),
		m_Color,
		XMFLOAT2(m_UV.eu, m_UV.ev),
	};

	// 頂点バッファ生成 //////
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.ByteWidth = sizeof(VERTEX_2D) * 4;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertices;
	CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBufer);

	// ファイルフォーマット検索
	std::string file = pFileName;
	std::regex tga(".tga", std::regex_constants::icase);
	std::regex png(".png", std::regex_constants::icase);
	std::smatch m;

	// テクスチャ読み込み //////
	m_Texture = new CTexture();
	if (std::regex_search(file, m, tga)) {
		m_Texture->Load(pFileName);
	}
	if (std::regex_search(file, m, png)) {
		m_Texture->LoadSTB(pFileName);
	}

	// シェーダー
	m_Shader = ShaderManager::GetShader<CShader2D>();
}

void CPolygon2D::Uninit()
{
	m_VertexBufer->Release();
	delete m_Texture;
}

void CPolygon2D::Draw()
{
	SetBuffer();

	UINT Stride = sizeof(VERTEX_2D);
	UINT offdet = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBufer, &Stride, &offdet);
	CRenderer::SetTexture(m_Texture);

	m_Shader->Set();

	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CRenderer::GetDeviceContext()->Draw(4, 0);
}

void CPolygon2D::SetBuffer()
{
	// 回転した後の4点を求める（1.0f / 2.0f = 0.5f）
	float px[4], py[4];
	px[0] = -0.5f * m_Size.x * cos(m_RotateDeg) + 0.5f * m_Size.y * sin(m_RotateDeg);
	py[0] = -0.5f * m_Size.x * sin(m_RotateDeg) - 0.5f * m_Size.y * cos(m_RotateDeg);
	px[1] =  0.5f * m_Size.x * cos(m_RotateDeg) + 0.5f * m_Size.y * sin(m_RotateDeg);
	py[1] =  0.5f * m_Size.x * sin(m_RotateDeg) - 0.5f * m_Size.y * cos(m_RotateDeg);
	py[2] = -0.5f * m_Size.x * sin(m_RotateDeg) + 0.5f * m_Size.y * cos(m_RotateDeg);
	px[2] = -0.5f * m_Size.x * cos(m_RotateDeg) - 0.5f * m_Size.y * sin(m_RotateDeg);
	px[3] =  0.5f * m_Size.x * cos(m_RotateDeg) - 0.5f * m_Size.y * sin(m_RotateDeg);
	py[3] =  0.5f * m_Size.x * sin(m_RotateDeg) + 0.5f * m_Size.y * cos(m_RotateDeg);

	// 頂点情報セット
	VERTEX_2D vertices[4] = {

		Vector3(px[0] + m_Position.x, py[0] + m_Position.y, 0.0f),
		m_Color,
		XMFLOAT2(m_UV.su, m_UV.sv),

		Vector3(px[1] + m_Position.x, py[1] + m_Position.y, 0.0f),
		m_Color,
		XMFLOAT2(m_UV.eu, m_UV.sv),

		Vector3(px[2] + m_Position.x, py[2] + m_Position.y, 0.0f),
		m_Color,
		XMFLOAT2(m_UV.su, m_UV.ev),

		Vector3(px[3] + m_Position.x, py[3] + m_Position.y, 0.0f),
		m_Color,
		XMFLOAT2(m_UV.eu, m_UV.ev),
	};

	// VertexBuffer書き換え
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		CRenderer::GetDeviceContext()->Map(m_VertexBufer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		memcpy(msr.pData, vertices, sizeof(VERTEX_2D) * 4);
		CRenderer::GetDeviceContext()->Unmap(m_VertexBufer, 0);
	}
}
