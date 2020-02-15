#include "main.h"
#include "shader_base.h"
#include "shader_2d.h"
#include "renderer.h"
#include "texture.h"
#include "image.h"

void CImage::Draw()
{
	CRenderer::SetDepthEnable(false);
	DrawBuffer();
	CRenderer::SetDepthEnable(true);
}

void CImage::DrawBuffer()
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


void CImage::SetBuffer()
{
	// 回転した後の4点を求める（1.0f / 2.0f = 0.5f）
	float px[4], py[4];
	px[0] = -0.5f * m_Size.x * cos(m_RotateDeg) + 0.5f * m_Size.y * sin(m_RotateDeg);
	py[0] = -0.5f * m_Size.x * sin(m_RotateDeg) - 0.5f * m_Size.y * cos(m_RotateDeg);
	px[1] = 0.5f * m_Size.x * cos(m_RotateDeg) + 0.5f * m_Size.y * sin(m_RotateDeg);
	py[1] = 0.5f * m_Size.x * sin(m_RotateDeg) - 0.5f * m_Size.y * cos(m_RotateDeg);
	py[2] = -0.5f * m_Size.x * sin(m_RotateDeg) + 0.5f * m_Size.y * cos(m_RotateDeg);
	px[2] = -0.5f * m_Size.x * cos(m_RotateDeg) - 0.5f * m_Size.y * sin(m_RotateDeg);
	px[3] = 0.5f * m_Size.x * cos(m_RotateDeg) - 0.5f * m_Size.y * sin(m_RotateDeg);
	py[3] = 0.5f * m_Size.x * sin(m_RotateDeg) + 0.5f * m_Size.y * cos(m_RotateDeg);

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
