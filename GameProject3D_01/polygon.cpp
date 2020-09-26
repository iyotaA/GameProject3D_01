
// インクルード ////////////////////////////////////
#include "main.h"
#include "shader_all.h"
#include "renderer.h"
#include "texture.h"
#include "gameObject.h"
#include "polygon.h"
#include <regex>
#include <iostream>


CPolygon2D::CPolygon2D(char* pFileName)
	: m_Position(XMFLOAT2(0.0f, 0.0f))
	, m_UV(UV())
	, m_Size(XMFLOAT2(50.0f, 50.0f))
	, m_Color(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f))
	, m_RotateDeg(0.0f)
	, m_Delete(false)
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
	//m_Shader = ShaderManager::GetShader<CShader2D>();
	m_Shader = new CShader2D();
	m_Shader->Init("asset/shader/shader2DVS.cso", "asset/shader/shader2DPS.cso");
}

CPolygon2D::~CPolygon2D()
{
	m_VertexBufer->Release();
	delete m_Texture;
}

