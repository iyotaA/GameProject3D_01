
// �C���N���[�h ////////////////////////////////////
#include "game_objects_all.h"
#include "scene.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"
#include "shader_all.h"
#include "skinmesh_animation.h"
#include "main.h"

#define Glavity (-0.098f)
#define Mass	(10.0f)

static double t = 0.0;

void CEnemy::Init()
{
	// ���f���̏�����
	m_pModel = new CSkinModel();
	m_pModel->Load("asset/model/dragon001.fbx", 0.55f, "asset/image/dragon.png", NULL);


	// �g�����X�t�H�[��������
	m_Position = Vector3(70.0f, 0.0f, -90.0f);
	//m_Rotation = Vector3(0.0f, 180.0f * 3.14f / 180.0f, 0.0f);
	m_Rotation = Vector3(0.0f, 180.0f * 3.14f / 180.0f, 0.0f);
	m_Scale = Vector3(1.0f, 1.0f, 1.0f);

	// Front_Up_Right�x�N�g��������
	m_DirVec.SetFrontUpRight(Vector3(0.0f, 0.0f, 1.0f));

	// �R���W�����̏�����
	m_CollisionSphere = new CCollisionSphere(Vector3( m_Position.x, m_Position.y + 7.0f, m_Position.z), 7.0f);
	m_CollisionOBB = new CCollisionOBB(m_Position, m_DirVec, Vector3(0.5f, 0.5f, 0.5f));

	// �_���[�W�֘A�X�e�[�^�X�̏�����
	m_DamageManager = new CDamage(100, 15);
	m_DamageManager->GetCollisionSphere()->SetRadius(0.2f);

	m_MoveDistance = Vector3(0.0f, 0.0f, 0.0f);
	m_BonePosition = Vector3(0.0f, 0.0f, 0.0f);
	m_MoveSpeed = m_DefaultSpeed;
	m_IsCollision = false;
	m_IsPressMovingEntry = false;
}

void CEnemy::Uninit()
{
	delete m_DamageManager;
	delete m_CollisionOBB;
	delete m_CollisionSphere;


	m_pModel->Unload();
	delete m_pModel;
}

void CEnemy::Update()
{

	// �ړ�
	Move();

	// �s��
	Action();

	// ���f���X�V
	m_pModel->update(1);

}

void CEnemy::Draw()
{
	// �J�����O
	//CCamera* camera;
	//camera = CManager::GetScene()->GetGameObject<CCamera>(CManager::E_Camera);

	//if (camera->GetVisivility(&m_Position) == false) {
	//	return;
	//}


	// �}�g���N�X�ݒ�
	XMMATRIX world;
	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	world *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	// ���f���`��
	m_pModel->Draw(&world);

	// �R���W�����`��
	DrawCollisionGrid();

	// ImGui�`��
	DrawGUI();
}


void CEnemy::DrawCollisionGrid()
{
	// �f�o�b�O�O���b�h�Z�b�g
	XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
	CDebugPrimitive::DebugPrimitive_BatchCirecleDraw(m_CollisionSphere, &color);

}

void CEnemy::DrawGUI()
{

}

void CEnemy::Move()
{
	// �d�͉��Z
	AddGlavity();
}


void CEnemy::Action()
{
}


void CEnemy::UpdateCollision()
{
	m_CollisionSphere->SetCenter(&Vector3(m_Position.x, m_Position.y + 7.0f, m_Position.z));
}

void CEnemy::AddGlavity()
{
	// �d��
	float move_y = Glavity * Mass * t * t * 0.5f;
	m_Position.y += move_y;

	// �n�`�Ƃ̏Փ˔���
	bool landing = IsLanding();
	if (landing) {
		t = 0.0f;
	}
	else {
		t += DELTA_TIME;
	}

	// �ŉ��w�i����ȉ��ɉ�����Ȃ��悤�Ɂj
	if (m_Position.y < -20.0f) { m_Position.y = -20.0f; }

}


bool CEnemy::IsLanding()
{
	// �n�ʂƂ̃R���W����
	CTerrain* pTerrain = CManager::GetScene()->GetGameObject<CTerrain>(CManager::E_Background);
	float height = pTerrain->GetHeight(&m_Position);
	if (FAILD_NUM != (int)height) {
		if (m_Position.y <= height) {
			m_Position.y = height;
			return true;
		}
		else {
			return false;
		}
	}
}

