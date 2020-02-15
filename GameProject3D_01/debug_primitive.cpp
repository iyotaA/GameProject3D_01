//=================================================================================
// インクルード
//=================================================================================
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "collision3D.h"
#include "stl.h"
#include "shader_all.h"
#include "debug_primitive.h"
#include "imGui_all.h"

#include "gameObject.h"
#include "camera_manager.h"
#include "camera.h"

//=================================================================================
// マクロ定義
//=================================================================================
#define CIRCLE_VERTEX_COUNT	(8)

std::vector<VERTEX_3D>	CDebugPrimitive::m_Vertices;
std::vector<WORD>			CDebugPrimitive::m_Indices;
ID3D11Buffer*					CDebugPrimitive::m_VertexBufer;
ID3D11Buffer*					CDebugPrimitive::m_IndexBufer;
CShaderDefault*				CDebugPrimitive::m_Shader;
CTexture*							CDebugPrimitive::m_Texture;

unsigned int	CDebugPrimitive::m_CircleCount;
unsigned int  CDebugPrimitive::m_CubeCount;
bool				CDebugPrimitive::m_IsDisplayed;


void CDebugPrimitive::DebugPrimitive_Init(void)
{
	m_CircleCount = 0;
	m_CubeCount = 0;
	m_IsDisplayed = true;

	m_Texture = new CTexture();
	m_Texture->LoadSTB("asset/image/white.png");

	m_Shader = ShaderManager::GetShader<CShaderDefault>();
}

void CDebugPrimitive::DebugPrimitive_Uninit(void)
{
	SAFE_RELEASE(m_VertexBufer);
	SAFE_RELEASE(m_IndexBufer);

	if (!m_Vertices.empty()) { m_Vertices.clear(); }
	if (!m_Indices.empty()) { m_Indices.clear(); }
}

void CDebugPrimitive::DebugPrimitive_BatchBegin(void)
{
	// 描画数をゼロに
	m_CircleCount = 0;
	m_CubeCount = 0;

	if(!m_Vertices.empty()){ m_Vertices.clear(); }
	if(!m_Indices.empty()){ m_Indices.clear(); }
}

void CDebugPrimitive::DebugPrimitive_BatchRun(void)
{
	//表示する時のみ
	if (!m_IsDisplayed)return;

	// ヴァーテクスバッファ生成 //////
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * m_Vertices.size();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Vertices.data();

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBufer);

		assert(m_VertexBufer);
	}

	// インデックスバッファ生成 //////
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(WORD) * m_Indices.size();
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Indices.data();
		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBufer);

		assert(m_IndexBufer);
	}

	XMMATRIX world;
	world = XMMatrixIdentity();
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

	CRenderer::SetTexture(m_Texture);
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBufer, &Stride, &offdet);	// バーテクスバッファセット
	CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBufer, DXGI_FORMAT_R16_UINT, 0);		// インデックスバッファセット
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	CRenderer::GetDeviceContext()->DrawIndexed((CIRCLE_VERTEX_COUNT * 2 * m_CircleCount) + (m_CubeCount * 24), 0, 0);

	SAFE_RELEASE(m_IndexBufer);
	SAFE_RELEASE(m_VertexBufer);
}


void CDebugPrimitive::DebugPrimitive_BatchCirecleDraw(CCollisionSphere* _sphere)
{
	//表示する時のみ
	if (!m_IsDisplayed)return;


	int n;
	float a = PI * 2.0f / CIRCLE_VERTEX_COUNT;

	// ( 円 1 ) //////////////////////////////////////////////////////////////
	n = (m_CubeCount + m_CircleCount) * CIRCLE_VERTEX_COUNT;

	for (int i = 0; i < CIRCLE_VERTEX_COUNT; i++) {
		VERTEX_3D vertex;
		vertex.Position.x		= cos(static_cast<double>(a) * static_cast<double>(i)) * _sphere->GetRadius() + _sphere->GetCenter().x;
		vertex.Position.y		= sin(static_cast<double>(a) * static_cast<double>(i)) * _sphere->GetRadius() + _sphere->GetCenter().y;
		vertex.Position.z		= _sphere->GetCenter().z;
		vertex.Normal		= Vector3(0.0f, 1.0f, 0.0f);
		vertex.Diffuse			= _sphere->GetColor();
		vertex.TexCoord		= XMFLOAT2(0.0f, 0.0f);

		m_Vertices.push_back(vertex);
	}

	for (int i = 0; i < CIRCLE_VERTEX_COUNT; i++) {

		WORD indices;

		indices = n + i;
		m_Indices.push_back(indices);

		indices = n + (i + 1) % CIRCLE_VERTEX_COUNT;
		m_Indices.push_back(indices);
	}
	m_CircleCount++;

	// ( 円 2 ) //////////////////////////////////////////////////////////////
	n = (m_CubeCount + m_CircleCount) * CIRCLE_VERTEX_COUNT;

	for (int i = 0; i < CIRCLE_VERTEX_COUNT; i++) {
		VERTEX_3D vertex;

		vertex.Position.x = _sphere->GetCenter().x;
		vertex.Position.y = cos(static_cast<double>(a) * static_cast<double>(i)) * _sphere->GetRadius() + _sphere->GetCenter().y;
		vertex.Position.z = sin(static_cast<double>(a) * static_cast<double>(i)) * _sphere->GetRadius() + _sphere->GetCenter().z;
		vertex.Normal = Vector3(0.0f, 1.0f, 0.0f);
		vertex.Diffuse = _sphere->GetColor();
		vertex.TexCoord = XMFLOAT2(0.0f, 0.0f);

		m_Vertices.push_back(vertex);
	}


	for (int i = 0; i < CIRCLE_VERTEX_COUNT; i++) {

		WORD indices;

		indices = n + i;
		m_Indices.push_back(indices);

		indices = n + (i + 1) % CIRCLE_VERTEX_COUNT;
		m_Indices.push_back(indices);
	}

	m_CircleCount++;

	// ( 円 3 ) //////////////////////////////////////////////////////////////
	n = (m_CubeCount + m_CircleCount) * CIRCLE_VERTEX_COUNT;

	for (int i = 0; i < CIRCLE_VERTEX_COUNT; i++) {
		VERTEX_3D vertex;

		vertex.Position.x = cos((static_cast<double>(a) * static_cast<double>(i))) * _sphere->GetRadius() + _sphere->GetCenter().x;
		vertex.Position.y = _sphere->GetCenter().y;
		vertex.Position.z = sin(static_cast<double>(a)* static_cast<double>(i)) * _sphere->GetRadius() + _sphere->GetCenter().z;
		vertex.Normal = Vector3(0.0f, 1.0f, 0.0f);
		vertex.Diffuse = _sphere->GetColor();
		vertex.TexCoord = XMFLOAT2(0.0f, 0.0f);

		m_Vertices.push_back(vertex);
	}

	for (int i = 0; i < CIRCLE_VERTEX_COUNT; i++) {

		WORD indices;

		indices = n + i;
		m_Indices.push_back(indices);

		indices = n + (i + 1) % CIRCLE_VERTEX_COUNT;
		m_Indices.push_back(indices);
	}

	m_CircleCount++;
}

void CDebugPrimitive::DebugPrimitive_BatchCubeDraw(CCollisionOBB* _cube)
{
	//表示する時のみ
	if (!m_IsDisplayed)return;

	Vector3 vFront, vRigth, vUp, length3, center;

	vFront  = _cube->GetDirect(2);
	vRigth  = _cube->GetDirect(0);
	vUp     = _cube->GetDirect(1);
	length3 = _cube->GetLen_W();
	center  = _cube->GetPos_W();


	VERTEX_3D vertex;

	//////////////////////////////////////////////
	// 後面
	//////////////////////////////////////////////
	// 0
	vertex.Position = Vector3(
		center.x + (-vFront.x * length3.z) + (vUp.x * length3.y) + (-vRigth.x * length3.x),
		center.y + (-vFront.y * length3.z) + (vUp.y * length3.y) + (-vRigth.y * length3.x),
		center.z + (-vFront.z * length3.z) + (vUp.z * length3.y) + (-vRigth.z * length3.x)
		);
	vertex.Normal   = Vector3(0.0f, 1.0f, 0.0f);
	vertex.Diffuse  = _cube->GetColor();
	vertex.TexCoord = XMFLOAT2(0.0f, 0.0f);
	m_Vertices.push_back(vertex);

	// 1
	vertex.Position = Vector3(
		center.x + (-vFront.x * length3.z) + (vUp.x * length3.y) + (vRigth.x * length3.x),
		center.y + (-vFront.y * length3.z) + (vUp.y * length3.y) + (vRigth.y * length3.x),
		center.z + (-vFront.z * length3.z) + (vUp.z * length3.y) + (vRigth.z * length3.x)
	);
	vertex.Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex.Diffuse = _cube->GetColor();
	vertex.TexCoord = XMFLOAT2(0.0f, 0.0f);
	m_Vertices.push_back(vertex);

	// 2
	vertex.Position = Vector3(
		center.x + (-vFront.x * length3.z) + (-vUp.x * length3.y) + (vRigth.x * length3.x),
		center.y + (-vFront.y * length3.z) + (-vUp.y * length3.y) + (vRigth.y * length3.x),
		center.z + (-vFront.z * length3.z) + (-vUp.z * length3.y) + (vRigth.z * length3.x)
	);
	vertex.Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex.Diffuse = _cube->GetColor();
	vertex.TexCoord = XMFLOAT2(0.0f, 0.0f);
	m_Vertices.push_back(vertex);

	// 3
	vertex.Position = Vector3(
		center.x + (-vFront.x * length3.z) + (-vUp.x * length3.y) + (-vRigth.x * length3.x),
		center.y + (-vFront.y * length3.z) + (-vUp.y * length3.y) + (-vRigth.y * length3.x),
		center.z + (-vFront.z * length3.z) + (-vUp.z * length3.y) + (-vRigth.z * length3.x)
	);
	vertex.Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex.Diffuse = _cube->GetColor();
	vertex.TexCoord = XMFLOAT2(0.0f, 0.0f);
	m_Vertices.push_back(vertex);

	//////////////////////////////////////////////
	// 前面
	//////////////////////////////////////////////
	// 4
	vertex.Position = Vector3(
		center.x + (vFront.x * length3.z) + (vUp.x * length3.y) + (-vRigth.x * length3.x),
		center.y + (vFront.y * length3.z) + (vUp.y * length3.y) + (-vRigth.y * length3.x),
		center.z + (vFront.z * length3.z) + (vUp.z * length3.y) + (-vRigth.z * length3.x)
	);
	vertex.Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex.Diffuse = _cube->GetColor();
	vertex.TexCoord = XMFLOAT2(0.0f, 0.0f);
	m_Vertices.push_back(vertex);

	// 5
	vertex.Position = Vector3(
		center.x + (vFront.x * length3.z) + (vUp.x * length3.y) + (vRigth.x * length3.x),
		center.y + (vFront.y * length3.z) + (vUp.y * length3.y) + (vRigth.y * length3.x),
		center.z + (vFront.z * length3.z) + (vUp.z * length3.y) + (vRigth.z * length3.x)
	);
	vertex.Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex.Diffuse = _cube->GetColor();
	vertex.TexCoord = XMFLOAT2(0.0f, 0.0f);
	m_Vertices.push_back(vertex);

	// 6
	vertex.Position = Vector3(
		center.x + (vFront.x * length3.z) + (-vUp.x * length3.y) + (vRigth.x * length3.x),
		center.y + (vFront.y * length3.z) + (-vUp.y * length3.y) + (vRigth.y * length3.x),
		center.z + (vFront.z * length3.z) + (-vUp.z * length3.y) + (vRigth.z * length3.x)
	);
	vertex.Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex.Diffuse = _cube->GetColor();
	vertex.TexCoord = XMFLOAT2(0.0f, 0.0f);
	m_Vertices.push_back(vertex);

	// 7
	vertex.Position = Vector3(
		center.x + (vFront.x * length3.z) + (-vUp.x * length3.y) + (-vRigth.x * length3.x),
		center.y + (vFront.y * length3.z) + (-vUp.y * length3.y) + (-vRigth.y * length3.x),
		center.z + (vFront.z * length3.z) + (-vUp.z * length3.y) + (-vRigth.z * length3.x)
	);
	vertex.Normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex.Diffuse = _cube->GetColor();
	vertex.TexCoord = XMFLOAT2(0.0f, 0.0f);
	m_Vertices.push_back(vertex);

	int n = (m_CubeCount + m_CircleCount) * CIRCLE_VERTEX_COUNT;

	m_Indices.push_back(n);
	m_Indices.push_back(n + 1);

	m_Indices.push_back(n + 1);
	m_Indices.push_back(n + 2);

	m_Indices.push_back(n + 2);
	m_Indices.push_back(n + 3);

	m_Indices.push_back(n + 3);
	m_Indices.push_back(n);

	m_Indices.push_back(n + 4);
	m_Indices.push_back(n + 5);

	m_Indices.push_back(n + 5);
	m_Indices.push_back(n + 6);

	m_Indices.push_back(n + 6);
	m_Indices.push_back(n + 7);

	m_Indices.push_back(n + 7);
	m_Indices.push_back(n + 4);

	m_Indices.push_back(n + 4);
	m_Indices.push_back(n);

	m_Indices.push_back(n + 5);
	m_Indices.push_back(n + 1);

	m_Indices.push_back(n + 6);
	m_Indices.push_back(n + 2);

	m_Indices.push_back(n + 7);
	m_Indices.push_back(n + 3);


	m_CubeCount++;
}


void CDebugPrimitive::DrawGUI()
{
	ImGui::Checkbox("CollisionGrid", &m_IsDisplayed);
}
