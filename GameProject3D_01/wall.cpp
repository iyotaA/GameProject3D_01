
// �C���N���[�h ////////////////////////////////////
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
	/* ���@�[�e�N�X���v�� */
	int VertexNum = (GridNumX + 1) * (GridNumZ + 1);

	/* �C���f�b�N�X���v�� */
	m_IndexNum = (2 + GridNumX * 2) * GridNumZ + (GridNumZ - 1) * 2;

	float ShiftedAmount_x = (float)SIZE * GridNumX / 2;	// ���b�V���t�B�[���h�̒��S�𐢊E�̒��S�ɂ��邽�߂ɂ��炷��
	float ShiftedAmount_z = (float)SIZE * GridNumZ / 2;	// ���b�V���t�B�[���h�̒��S�𐢊E�̒��S�ɂ��邽�߂ɂ��炷��

	// ���@�[�e�N�X���i�[
	{
		VERTEX_3D* pVertex;
		pVertex = new VERTEX_3D[VertexNum];

		for (int z = 0; z < GridNumZ + 1; z++) {
			for (int x = 0; x < GridNumX + 1; x++) {
				float V;
				/* V���W�iUV�j�ݒ� */
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
		/* �C���f�b�N�X���i�[ */
		for (int z = 0, i = 0; z < GridNumZ; z++) {
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

	// �e�N�X�`���ǂݍ��� //////
	m_Texture = new CTexture();
	m_Texture->LoadSTB("asset/image/dart000.png");

	// �g�����X�t�H�[��������
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
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBufer, &Stride, &offdet);	// �o�[�e�N�X�o�b�t�@�Z�b�g
	CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBufer, DXGI_FORMAT_R16_UINT, 0);		// �C���f�b�N�X�o�b�t�@�Z�b�g
	CRenderer::SetTexture(m_Texture);
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CRenderer::GetDeviceContext()->DrawIndexed(m_IndexNum, 0, 0);
}

