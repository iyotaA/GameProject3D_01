
// �C���N���[�h ////////////////////////////////////
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "2dAnimation.h"

ID3D11Buffer*	C2DAnimation::m_VertexBufer;
CTexture*		C2DAnimation::m_Texture;

void C2DAnimation::Draw(XMFLOAT2 position, XMFLOAT2 uv0, XMFLOAT2 cutSize)
{
	// �e�N�X�`���̕��E�����擾
	XMINT2 size = *m_Texture->GetSize();

	// UV���W�ݒ�
	float u0 = uv0.x;
	float v0 = uv0.y;
	float u1 = (float)(uv0.x + (float)(cutSize.x / size.x));
	float v1 = (float)(uv0.y + (float)(cutSize.y / size.y));

	// ���_���W�ݒ�
	VERTEX_3D vertex[4] = {

		vertex[0].Position = Vector3(position.x, position.y, 0.0f),	// "XM"�����ɂ��Ă�ϐ��E�֐���DirectXMath���C�u�����̈ꕔ
		vertex[0].Normal = Vector3(0.0f, 0.0f, -1.0f),
		vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		vertex[0].TexCoord = XMFLOAT2(u0, v0),

		vertex[1].Position = Vector3(position.x + cutSize.x, position.y, 0.0f),
		vertex[1].Normal = Vector3(0.0f, 0.0f, -1.0f),
		vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		vertex[1].TexCoord = XMFLOAT2(u1, v0),

		vertex[2].Position = Vector3(position.x, position.y + cutSize.y, 0.0f),
		vertex[2].Normal = Vector3(0.0f, 0.0f, -1.0f),
		vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		vertex[2].TexCoord = XMFLOAT2(u0, v1),

		vertex[3].Position = Vector3(position.x + cutSize.x, position.y + cutSize.y, 0.0f),
		vertex[3].Normal = Vector3(0.0f, 0.0f, -1.0f),
		vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		vertex[3].TexCoord = XMFLOAT2(u1, v1),
	};

	// ���_�o�b�t�@���� //////
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


	UINT Stride = sizeof(VERTEX_3D);
	UINT offdet = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBufer, &Stride, &offdet);
	CRenderer::SetTexture(m_Texture);
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CRenderer::GetDeviceContext()->Draw(4, 0);

	if (m_VertexBufer)	m_VertexBufer->Release();
	if (m_Texture)		m_Texture->Unload();
}
