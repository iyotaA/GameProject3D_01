
// �C���N���[�h ////////////////////////////////////
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "gameObject.h"
#include "skyDome.h"

#define Radius		(300.0f)
#define Height		(100)
#define GridNumX	(500)
#define GridNumZ	(60)

void CSkyDome::Init()
{
	/* ���@�[�e�N�X���v�� */
	int VertexNum = (GridNumX + 1) * (GridNumZ + 1);

	/* �C���f�b�N�X���v�� */
	m_IndexNum = (2 + GridNumX * 2) * (GridNumZ + 1) + GridNumZ * 2;

	// ���@�[�e�N�X���i�[
	{
		VERTEX_3D* pVertex;
		pVertex = new VERTEX_3D[ VertexNum];

		for (int z = 0; z < GridNumZ + 1; z++) {
			for (int x = 0; x < GridNumX + 1; x++) {
				float radius;			// �h�[���̔��a�i���X�ɏ������Ȃ�j

				if (z == GridNumZ) {	// �Ō�̗�̒��_
					radius = 0.0f;
				}
				else {                      // �Ō�ȊO�̗�
					radius = cosf(((90 / (GridNumZ)) * z) * PI / 180.0f) * Radius;
				}


				float height = sinf(((90  / GridNumZ) * z) * PI / 180.0f) * Height;
				float Pos_x  = cosf(((360.0f / GridNumX) * x) * PI / 180.0f) * radius;
				float Pos_z  = sinf(((360.0f / GridNumX) * x) * PI / 180.0f) * radius;

				if (x == 0) {
					pVertex[x + (GridNumX + 1) * z] = {
						XMFLOAT3(Pos_x, height, Pos_z),
						XMFLOAT3(0.0f, 1.0f, 0.0f),
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
						XMFLOAT2((float)x * 2 / GridNumX + 0.01f, (float)(GridNumZ - z) / GridNumZ)
					};
				}
				else if (x == GridNumX) {
					pVertex[x + (GridNumX + 1) * z] = {
						XMFLOAT3(Pos_x, height, Pos_z),
						XMFLOAT3(0.0f, 1.0f, 0.0f),
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
						XMFLOAT2((float)x * 2 / GridNumX - 0.01f, (float)(GridNumZ - z) / GridNumZ)
					};
				}
				else {
					pVertex[x + (GridNumX + 1) * z] = {
						XMFLOAT3(Pos_x, height, Pos_z),
						XMFLOAT3(0.0f, 1.0f, 0.0f),
						XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
						XMFLOAT2((float)x * 2 / GridNumX, (float)(GridNumZ - z) / GridNumZ)
					};
				}
			}
		}
		// ���@�[�e�N�X�o�b�t�@���� //////
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

	// �C���f�b�N�X���i�[
	{
		WORD* pIndex;
		pIndex = new WORD[m_IndexNum];

		for (int z = 0, i = 0; z < GridNumZ + 1; z++) {
			for (int x = 0; x < GridNumX + 1; x++) {

				if (x == GridNumX) {/* �E�[�̒��_���i�s�̏I���j */
					pIndex[i] = x + (GridNumX + 1) * z + (GridNumX + 1);
					i++;
					pIndex[i] = x + (GridNumX + 1) * z;
					i++;
					if (i < m_IndexNum) {/* ��ԍŌ�̃C���f�b�N�X���i�[����Ă���Ώk�ރ|���S���͍��Ȃ� */
						pIndex[i] = x + (GridNumX + 1) * z;
						i++;
					}
				}
				else if (z && !x) {	/* ���[�̒��_���i�s�̎n�߁j */
					pIndex[i] = x + (GridNumX + 1) * z + (GridNumX + 1);
					i++;
					pIndex[i] = x + (GridNumX + 1) * z + (GridNumX + 1);
					i++;
					pIndex[i] = x + (GridNumX + 1) * z;
					i++;
				}
				else {				/* ����ȊO�̒��_��� */
					pIndex[i] = x + (GridNumX + 1) * z + (GridNumX + 1);
					i++;
					pIndex[i] = x + (GridNumX + 1) * z;
					i++;
				}
			}
		}
		// �C���f�b�N�X�o�b�t�@���� //////
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

	// �e�N�X�`���ǂݍ��� /////
	m_Texture = new CTexture();
	m_Texture->LoadSTB("asset/sky001.tga");

	// �g�����X�t�H�[��������
	m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
}

void CSkyDome::Uninit()
{
	m_VertexBufer->Release();
	m_IndexBufer->Release();
	delete m_Texture;
}

void CSkyDome::Update()
{
	m_Rotation.y += XMConvertToRadians(0.005f);
}

void CSkyDome::Draw()
{
	XMMATRIX world;
	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	world *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	CRenderer::SetWorldMatrix(&world);

	UINT Stride = sizeof(VERTEX_3D);
	UINT offdet = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBufer, &Stride, &offdet);	// �o�[�e�N�X�o�b�t�@�Z�b�g
	CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBufer, DXGI_FORMAT_R16_UINT, 0);		// �C���f�b�N�X�o�b�t�@�Z�b�g
	CRenderer::SetTexture(m_Texture);
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CRenderer::GetDeviceContext()->DrawIndexed(m_IndexNum, 0, 0);
}
