
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
#include "state_enemy_idle.h"

#define Glavity (-0.098f)
#define Mass	(10.0f)

static double t = 0.0;

void CEnemy::Init()
{
	//---   ���f���̏�����   ---------------------------------------------------------------------------------
	m_pModel = new CSkinModel();
	m_pModel->Load("asset/model/dragon.fbx", 2.0f, "asset/image/dragon.png", "asset/NodeNameFiles/doragon.txt");


	//---   ���   ---------------------------------------------------------------------------------------------
	m_pState = new CStateEnemyIdle(this);


	//---   �e   -----------------------------------------------------------------------------------------------
	m_Shadow = new CPolygon3D();
	m_Shadow->Init(Vector3(0.0f, 0.0f, 0.0f), Vector3(6.0f, 1.0f, 12.0f), Vector3(0.0f, 0.0f, 0.0f), "asset/image/shadow.png");


	//---   �g�����X�t�H�[��������   -------------------------------------------------------------------------
	m_Position = Vector3(-10.0f, 0.0f, 30.0f);
	m_Rotation = Vector3(0.0f, 180.0f * 3.14f / 180.0f, 0.0f);
	m_Scale = Vector3(1.0f, 1.0f, 1.0f);


	//---   Front_Up_Right�x�N�g��������   -------------------------------------------------------------------
	m_DirVec.SetFrontUpRight(Vector3(0.0f, 0.0f, 1.0f));


	//---   �R���W�����̏�����   -----------------------------------------------------------------------------
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 1.5f), "B_Head"));				// ��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 1.0f), "B_Chin"));				// �{
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 2.0f), "B_Hip"));				// �K
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 2.0f), "B_Pelvis"));			// ��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 2.5f), "B_Spine"));			// �w��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 1.0f), "B_R_Back_Leg"));	// ���E��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 1.0f), "B_L_Back_Leg"));	// ��덶��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 1.0f), "B_R_Back_Foot"));	// ���E��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 1.0f), "B_L_Back_Foot"));	// ��덶��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 1.0f), "B_R_Front_Leg"));	// �O�E�r
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 1.0f), "B_L_Front_Leg"));	// �O���r
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 0.7f), "B_R_Front_Foot")); // �O�E��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 0.7f), "B_L_Front_Foot")); // �O����
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 1.5f), "B_Tail.001"));			// �K��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 1.0f), "B_Tail.002"));			// �K��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 0.8f), "B_Tail.003"));			// �K��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 0.8f), "B_Tail.004"));			// �K��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 0.8f), "B_Tail.005"));			// �K��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 0.8f), "B_Tail.006"));			// �K��

	m_CollisionOBB = new CCollisionOBB(m_Position, m_DirVec, Vector3(1.0f, 2.0f, 1.0f));


	//---   ���̑������o�̏�����   ----------------------------------------------------------------------------
	m_MoveDistance = Vector3(0.0f, 0.0f, 0.0f);
	m_MoveSpeed = m_DEFAULT_SPEED;
	m_IsCollision = false;
	m_IsPressMovingEntry = false;

	m_StateFlags.Damage = false;
	m_StateFlags.Dodge = false;
	m_StateFlags.Attack = false;
	m_StateFlags.Block = false;
}

void CEnemy::Uninit()
{
	delete m_CollisionOBB;
	m_CollisionBody.clear();

	m_Shadow->Uninit();
	delete m_Shadow;

	delete m_pState;

	m_pModel->Unload();
	delete m_pModel;
}

void CEnemy::Update()
{
	// ��ԍX�V
	m_pState->Update(this);

	// �ړ�
	Move();

	// �s��
	Action();

	// ���f���X�V
	m_pModel->update(1);
}

void CEnemy::Draw()
{
	// �}�g���N�X�ݒ�
	XMMATRIX world;
	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	world *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	// ���f���`��
	m_pModel->Draw(&world);

	// �e�`��
	m_Shadow->Draw();

	// �R���W�����`��
	DrawCollisionGrid();

	// ImGui�`��
	DrawGUI();
}


void CEnemy::DrawCollisionGrid()
{
	// �f�o�b�O�O���b�h�Z�b�g
	XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
	for (CCollisionWithBone* coll : m_CollisionBody) {
		CDebugPrimitive::DebugPrimitive_BatchCirecleDraw(coll->GetSphere(), &color);
	}

	color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	CDebugPrimitive::DebugPrimitive_BatchCubeDraw(m_CollisionOBB, &color);
}

void CEnemy::DrawGUI()
{

}

void CEnemy::Move()
{
	// �d�͉��Z
	AddGlavity();

	//m_Rotation.y -= DEGREE_TO_RADIAN * 0.5f;
	// �����x�N�g����]
	Vector3 front = Vector3(0.0f, 0.0f, 1.0f);
	XMMATRIX rotationMtx;
	rotationMtx = XMMatrixRotationY(m_Rotation.y);

	front = XMVector3TransformNormal(front, rotationMtx);
	front.Normalize();
	m_DirVec.front = front;
	m_DirVec.right = -m_DirVec.front.VCross(m_DirVec.up);
	m_DirVec.right.Normalize();


	//m_Position += m_DirVec.front * 0.4f;

	// �e�̍X�V�i���̂̈ʒu�Ɉړ��j
	Vector3 pos = m_CollisionBody[3]->GetSphere()->GetCenter();
	m_Shadow->SetPosition(&Vector3(pos.x, 0.01f, pos.z));
	m_Shadow->SetRotation(&Vector3(m_Rotation.x, m_Rotation.y, m_Rotation.z));
	m_Shadow->Update();

	UpdateCollision();
}


void CEnemy::Action()
{
}


void CEnemy::UpdateCollision()
{
	// �}�g���N�X�ݒ�
	XMMATRIX world;
	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	world *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	// ����̃{�[���̈ʒu���擾
	for (CCollisionWithBone* coll : m_CollisionBody) {

		coll->GetSphere()->SetCenter(&m_pModel->GetWorldPosition(&world, coll->GetBoneName()));
	}
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

bool CEnemy::CurrentAnimationFinish(void)const
{
	return m_pModel->CurrentAnimationFinish();
}

int CEnemy::GetCurrentAnimFrameNum()const
{
	return m_pModel->GetCurrentAnimFrameNum();
}

bool& CEnemy::AnimationBlending(void)
{
	return m_pModel->AnimationBlending();
}

void CEnemy::SetAnimation(const unsigned int _id, const float _startBlendNum)
{
	m_pModel->SetAnimation(_id, _startBlendNum);
}

void CEnemy::SetAnimationSpeed(const float _speed)
{
	m_pModel->SetAnimationSpeed(_speed);
}

void CEnemy::ChangeState(CStateEnemy* pState)
{
	delete m_pState;
	m_pState = pState;
}

