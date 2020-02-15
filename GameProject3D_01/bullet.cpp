
// �C���N���[�h ////////////////////////////////////
#include "game_objects_all.h"
#include "scene.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"

#define SCALE	(0.3f)

void CBullet::Init()
{
	// ���f���̏�����
	m_pModel = new CModel;
	m_pModel->Load("asset/miku_01.obj");

	// �g�����X�t�H�[��������
	m_Position = Vector3(0.0f, 1.0f, 0.0f);
	m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
	m_Scale = Vector3(SCALE, SCALE, SCALE);

	// �R���W�����̏�����
	m_CollisionSphere = new CCollisionSphere;
	m_CollisionSphere->SetRadius(SCALE);
}

void CBullet::Uninit()
{
	// ���f���̏I������
	m_pModel->Unload();
	delete m_pModel;
}

void CBullet::Update()
{
	std::vector<CEnemy*> enemys;

	enemys = CManager::GetScene()->GetGameObjects<CEnemy>(CManager::LAYER_OBJECT);

	// �R���W�����ʒu�̍X�V
	m_CollisionSphere->SetCenter(&m_Position);

	//// �G�ƒe�̓����蔻��
	//for (CEnemy* enemy : enemys) {
	//	if (enemy != nullptr) {

	//		// �����蔻��
	//		if (CCollision3DJudge::Collision3D_Spher_Spher(enemy->GetCollisionSphere(), m_CollisionSphere)) {

	//			CSound::Play(SOUND_LABEL_SE_HIT);

	//			CManager::GetScene()->DestroyGameObject(enemy);
	//			CManager::GetScene()->DestroyGameObject(this);
	//			return;
	//		}
	//	}
	//}

	// �ǂ̊O�ŏ���
	if (fabs(m_Position.x) > 15.0f || fabs(m_Position.z) > 15.0f ){
		CManager::GetScene()->DestroyGameObject(this);
		return;
	}

	XMFLOAT3 direction;
	XMStoreFloat3(&direction, m_Direction.front);	// �J�����̑O�����Q�b�g

	m_Position.x += direction.x * 0.5f;
	//m_Position.y += direction.y;
	m_Position.z += direction.z * 0.5f;

}

void CBullet::Draw()
{

	// �}�g���N�X�ݒ�
	XMMATRIX world;
	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	world *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	m_pModel->Draw(&world);

	// �f�o�b�O�O���b�h�Z�b�g
	XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	CDebugPrimitive::DebugPrimitive_BatchCirecleDraw(m_CollisionSphere);
}

