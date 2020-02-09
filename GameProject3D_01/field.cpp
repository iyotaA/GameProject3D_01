// �C���N���[�h ////////////////////////////////////
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "gameObject.h"
#include "shader_all.h"
#include "field.h"
#include "camera_manager.h"
#include "camera.h"


#define SIZE		(4.0f)
#define GridNumX	(175)
#define GridNumZ	(175)
#define PlaneHeight (1.0f)


void CField::Init()
{
	/* ���@�[�e�N�X���v�� */
	int VertexNum = (GridNumX + 1) * (GridNumZ + 1);

	/* �C���f�b�N�X���v�� */
	m_IndexNum = (2 + GridNumX * 2) * GridNumZ + (GridNumZ - 1) * 2;

	float ShiftedAmount_x = (float)SIZE * GridNumX / 2;	// ���b�V���t�B�[���h�̒��S�𐢊E�̒��S�ɂ��邽�߂ɂ��炷��
	float ShiftedAmount_z = (float)SIZE * GridNumZ / 2;	// ���b�V���t�B�[���h�̒��S�𐢊E�̒��S�ɂ��邽�߂ɂ��炷��

	// ���@�[�e�N�X���i�[
	{
		m_Vertex = new VERTEX_3D_NOMAL_MAP[VertexNum];

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

				m_Vertex[x + (GridNumX + 1) * z] = {
					Vector3(x * SIZE - ShiftedAmount_x, PlaneHeight, -z * SIZE + ShiftedAmount_z),
					Vector3(0.0f, 0.0f, 1.0f),
					Vector3(1.0f, 0.0f, 0.0f),
					Vector3(0.0f, 1.0f, 0.0f),
					XMFLOAT4(0.0f, 0.5f, 1.0f, 0.3f),
					XMFLOAT2(x, z)
				};
			}
		}

		// �@���v�Z
		for (int z = 1; z < GridNumZ; z++) {
			for (int x = 1; x < GridNumX; x++) {

				Vector3 va, vb, vc;
				va.x = m_Vertex[x + 1 + (GridNumX + 1) * z].Position.x - m_Vertex[x - 1 + (GridNumX + 1) * z].Position.x;
				va.y = m_Vertex[x + 1 + (GridNumX + 1) * z].Position.y - m_Vertex[x - 1 + (GridNumX + 1) * z].Position.y;
				va.z = m_Vertex[x + 1 + (GridNumX + 1) * z].Position.z - m_Vertex[x - 1 + (GridNumX + 1) * z].Position.z;
				vb.x = m_Vertex[x + (GridNumX + 1) + (GridNumX + 1) * z].Position.x - m_Vertex[x - (GridNumX + 1) + (GridNumX + 1) * z].Position.x;
				vb.y = m_Vertex[x + (GridNumX + 1) + (GridNumX + 1) * z].Position.y - m_Vertex[x - (GridNumX + 1) + (GridNumX + 1) * z].Position.y;
				vb.z = m_Vertex[x + (GridNumX + 1) + (GridNumX + 1) * z].Position.z - m_Vertex[x - (GridNumX + 1) + (GridNumX + 1) * z].Position.z;

				vc.x = va.y * vb.z - va.z * vb.y;
				vc.y = va.z * vb.x - va.x * vb.z;
				vc.z = va.x * vb.y - va.y * vb.x;

				float length;
				length = sqrtf(vc.x * vc.x + vc.y * vc.y + vc.z * vc.z);	// �i�O�����̒藝�j���������߂�

				// �x�N�g���̐��K��
				vc.x = vc.x / length;
				vc.y = vc.y / length;
				vc.z = vc.z / length;

				m_Vertex[x + (GridNumX + 1) * z].Normal = vc;
			}
		}

		// ���@�[�e�N�X�o�b�t�@���� //////
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D_NOMAL_MAP) * VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Vertex;
		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBufer);
	}

	// �C���f�b�N�X���i�[
	{
		UINT* pIndex;
		pIndex = new UINT[m_IndexNum];
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
		bd.ByteWidth = sizeof(UINT) * m_IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = pIndex;
		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBufer);
	}

	// �g�p�V�F�[�_�[�Z�b�g
	m_Shader = ShaderManager::GetShader<CShaderNormalMap>();

	// �e�N�X�`���ǂݍ��� //////
	m_Texture = new CTexture*[m_TextureNum];
	m_Texture[0] = new CTexture();
	m_Texture[1] = new CTexture();
	m_Texture[2] = new CTexture();
	m_Texture[0]->LoadSTB("asset/image/white.png");
	m_Texture[1]->LoadSTB("asset/image/dart000.png");
	m_Texture[2]->LoadSTB("asset/image/normal_map/terrian000.png");

	// �g�����X�t�H�[��������
	m_Position = Vector3(0.0f, 0.0f, 0.0f);
	m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
	m_Scale = Vector3(1.0f, 1.0f, 1.0f);

}

void CField::Uninit()
{
	m_VertexBufer->Release();
	m_IndexBufer->Release();
	delete m_Texture;
}

void CField::Update()
{
	int numVertices = (GridNumX + 1) * (GridNumZ + 1);

	for (int i = 0; i < numVertices; i++) {
		m_Vertex[i].TexCoord = XMFLOAT2(m_Vertex[i].TexCoord.x + 0.005f, m_Vertex[i].TexCoord.y + 0.0f);
	}

	// VertexBuffer��������
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		CRenderer::GetDeviceContext()->Map(m_VertexBufer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		memcpy(msr.pData, m_Vertex, sizeof(VERTEX_3D_NOMAL_MAP) * numVertices);
		CRenderer::GetDeviceContext()->Unmap(m_VertexBufer, 0);
	}
}

void CField::Draw()
{
	XMMATRIX world;
	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	world *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	XMFLOAT4X4 world_4x4;
	XMFLOAT4X4 mtxWIT4x4;
	XMMATRIX mtxWIT;

	// ���[���h�s��ϊ�
	XMStoreFloat4x4(&world_4x4, world);

	// World * Inverse * Transpose
	mtxWIT = XMMatrixInverse(nullptr, world);
	mtxWIT = XMMatrixTranspose(mtxWIT);
	XMStoreFloat4x4(&mtxWIT4x4, mtxWIT);

	m_Shader->SetMtxWIT(&mtxWIT4x4);

	CCamera* camera = CCameraManager::GetCamera();

	m_Shader->SetWorldMatrix(&world_4x4);
	m_Shader->SetViewMatrix(&camera->GetViewMatrix());
	m_Shader->SetProjectionMatrix(&camera->GetProjectionMatrix());
	XMFLOAT4 camera_pos = XMFLOAT4(camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z, 1.0f);
	m_Shader->SetCameraPosition(&camera_pos);
	m_Shader->SetLight(LIGHT());

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = COLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_Shader->SetMaterial(material);
	m_Shader->SetSpequlerPow(0.01f);

	m_Shader->Set();

	UINT Stride = sizeof(VERTEX_3D_NOMAL_MAP);
	UINT offdet = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBufer, &Stride, &offdet);	// �o�[�e�N�X�o�b�t�@�Z�b�g
	CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBufer, DXGI_FORMAT_R32_UINT, 0);		// �C���f�b�N�X�o�b�t�@�Z�b�g
	CRenderer::SetTexture(m_Texture, 0, m_TextureNum);
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CRenderer::GetDeviceContext()->DrawIndexed(m_IndexNum, 0, 0);
}

float CField::GetHeight(XMFLOAT3* _position)
{
	XMFLOAT3 position = *_position;

	if (!((position.x >= m_Vertex[0].Position.x && position.x <= m_Vertex[(GridNumX + 1) * (GridNumZ + 1) - 1].Position.x) &&
		(position.z <= m_Vertex[0].Position.z && position.z >= m_Vertex[(GridNumX + 1) * (GridNumZ + 1) - 1].Position.z)))
	{
		return FAILD_NUM;
	}

	int x, z;
	XMFLOAT3 p0, p1, p2, v01, v02, n, v, hp, va, vb;
	float dp0n, dvn, dpn, t;

	// �v���C���[����L�т鉺�����̃x�N�g��
	v = XMFLOAT3(0.0f, -1.0f, 0.0f);

	// �v���C���[�̍��W�𕝂Ŋ���ƃC���f�b�N�X���擾�ł���
	x = position.x / SIZE + GridNumX / 2;
	z = -position.z / SIZE + GridNumZ / 2;

	// va : �l�p�|���S���i�O�p*2�j�̑Ίp���̃x�N�g��
	// vb : �l�p�|���S���i�O�p*2�j�̍�����W����v���C���[�̈ʒu�܂ł̃x�N�g��
	va.x = m_Vertex[(x + 1) + (GridNumX + 1) * (z + 1)].Position.x - m_Vertex[x + (GridNumX + 1) * z].Position.x;
	va.y = m_Vertex[(x + 1) + (GridNumX + 1) * (z + 1)].Position.y - m_Vertex[x + (GridNumX + 1) * z].Position.y;
	va.z = m_Vertex[(x + 1) + (GridNumX + 1) * (z + 1)].Position.z - m_Vertex[x + (GridNumX + 1) * z].Position.z;
	vb.x = position.x - m_Vertex[x + (GridNumX + 1) * z].Position.x;
	vb.y = position.y - m_Vertex[x + (GridNumX + 1) * z].Position.y;
	vb.z = position.z - m_Vertex[x + (GridNumX + 1) * z].Position.z;

	// va �~ vb �� y ���� > 0.0f
	if ((va.z * vb.x - va.x * vb.z > 0.0f) || (va.z * vb.x - va.x * vb.z == 0.0f)) {

		p0 = m_Vertex[x + (GridNumX + 1) * (z + 1)].Position;		// p1|�_
		p1 = m_Vertex[x + (GridNumX + 1) * z].Position;				//   |�@�_
		p2 = m_Vertex[(x + 1) + (GridNumX + 1) * (z + 1)].Position; // p0|____�_p2
	}
	// va �~ vb �� y ���� < 0.0f
	else {
		p0 = m_Vertex[(x + 1) + (GridNumX + 1) * z].Position;       // p2�_'''''|p0
		p1 = m_Vertex[(x + 1) + (GridNumX + 1) * (z + 1)].Position; //     �_   |
		p2 = m_Vertex[x + (GridNumX + 1) * z].Position;             //   �@   �_|p1
	}

	// p1 - p0
	v01 = XMFLOAT3(p1.x - p0.x, p1.y - p0.y, p1.z - p0.z);

	// p2 - p0
	v02 = XMFLOAT3(p2.x - p0.x, p2.y - p0.y, p2.z - p0.z);

	// v01 �~�@v02
	n = XMFLOAT3(v01.y * v02.z - v01.z * v02.y, v01.z * v02.x - v01.x * v02.z, v01.x * v02.y - v01.y * v02.x);

	// v �E n
	dvn = v.x * n.x + v.y * n.y + v.z * n.z;

	// p0 �E n
	dp0n = p0.x * n.x + p0.y * n.y + p0.z * n.z;

	// position�En
	dpn = position.x * n.x + position.y * n.y + position.z * n.z;

	t = (dp0n - dpn) / dvn;

	// position + v * t
	hp = XMFLOAT3(position.x + v.x * t, position.y + v.y * t, position.z + v.z * t);
	//hp.y = position.y + v.y * t;

	return hp.y;
}

