
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
	m_pModel->Load("asset/model/dragon.fbx", 2.0f, "asset/image/dragon.png", "asset/NodeNameFiles/doragon.txt");
	m_pModel->SetAnimationSpeed(2.0f);

	// �e
	m_Shadow = new CPolygon3D();
	m_Shadow->Init(Vector3(0.0f, 0.0f, 0.0f), Vector3(6.0f, 1.0f, 12.0f), Vector3(0.0f, 0.0f, 0.0f), "asset/image/shadow.png");

	// �g�����X�t�H�[��������
	m_Position = Vector3(70.0f, 0.0f, -90.0f);
	//m_Rotation = Vector3(0.0f, 180.0f * 3.14f / 180.0f, 0.0f);
	m_Rotation = Vector3(0.0f, 180.0f * 3.14f / 180.0f, 0.0f);
	m_Scale = Vector3(1.0f, 1.0f, 1.0f);

	// Front_Up_Right�x�N�g��������
	m_DirVec.SetFrontUpRight(Vector3(0.0f, 0.0f, 1.0f));

	// �R���W�����̏�����
	m_CollisionSphere.push_back(new CCollisionSphere(Vector3( m_Position.x, m_Position.y + 7.0f, m_Position.z), 7.0f));
	m_CollisionSphere.push_back(new CCollisionSphere(Vector3( m_Position.x, m_Position.y + 7.0f, m_Position.z), 7.0f));
	m_CollisionSphere.push_back(new CCollisionSphere(Vector3( m_Position.x, m_Position.y + 7.0f, m_Position.z), 7.0f));
	m_CollisionSphere.push_back(new CCollisionSphere(Vector3( m_Position.x, m_Position.y + 7.0f, m_Position.z), 7.0f));
	m_CollisionSphere.push_back(new CCollisionSphere(Vector3( m_Position.x, m_Position.y + 7.0f, m_Position.z), 7.0f));
	m_CollisionSphere.push_back(new CCollisionSphere(Vector3( m_Position.x, m_Position.y + 7.0f, m_Position.z), 7.0f));
	m_CollisionOBB = new CCollisionOBB(m_Position, m_DirVec, Vector3(1.0f, 2.0f, 1.0f));

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
	m_CollisionSphere.clear();

	m_Shadow->Uninit();
	delete m_Shadow;

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
	for (CCollisionSphere* coll : m_CollisionSphere) {
		CDebugPrimitive::DebugPrimitive_BatchCirecleDraw(coll, &color);
	}

	color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	CDebugPrimitive::DebugPrimitive_BatchCirecleDraw(m_DamageManager->GetCollisionSphere(), &color);

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

	m_Rotation.y -= DEGREE_TO_RADIAN;
	// �����x�N�g����]
	Vector3 front = Vector3(0.0f, 0.0f, 1.0f);
	XMMATRIX rotationMtx;
	rotationMtx = XMMatrixRotationY(m_Rotation.y);

	front = XMVector3TransformNormal(front, rotationMtx);
	front.Normalize();
	m_DirVec.front = front;
	m_DirVec.right = -m_DirVec.front.VCross(m_DirVec.up);
	m_DirVec.right.Normalize();


	m_Position += m_DirVec.front * 0.4f;

	// �e�̍X�V
	m_Shadow->SetPosition(&Vector3(m_Position.x, 0.01f, m_Position.z));
	m_Shadow->SetRotation(&Vector3(m_Rotation.x, 0.0f, m_Rotation.z));
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
	m_BonePosition = m_pModel->GetWorldPosition(&world, "B_R_Back_Leg");
	m_DamageManager->GetCollisionSphere()->SetCenter(&m_pModel->GetWorldPosition(&world, "B_Head"));
	m_DamageManager->GetCollisionSphere()->SetRadius(2.0f);
	m_CollisionSphere[0]->SetCenter(&m_pModel->GetWorldPosition(&world, "B_Hip"));
	m_CollisionSphere[0]->SetRadius(3.0f);
	m_CollisionSphere[1]->SetCenter(&m_pModel->GetWorldPosition(&world, "B_Spine.001"));
	m_CollisionSphere[1]->SetRadius(3.0f);
	m_CollisionSphere[2]->SetCenter(&m_pModel->GetWorldPosition(&world, "B_R_Back_Leg"));
	m_CollisionSphere[2]->SetRadius(2.0f);
	m_CollisionSphere[3]->SetCenter(&m_pModel->GetWorldPosition(&world, "B_L_Back_Leg"));
	m_CollisionSphere[3]->SetRadius(2.0f);
	m_CollisionSphere[4]->SetCenter(&m_pModel->GetWorldPosition(&world, "B_R_Front_Leg"));
	m_CollisionSphere[4]->SetRadius(2.0f);
	m_CollisionSphere[5]->SetCenter(&m_pModel->GetWorldPosition(&world, "B_L_Front_Leg"));
	m_CollisionSphere[5]->SetRadius(2.0f);
	m_CollisionOBB->SetPosition(&m_BonePosition);

	//m_CollisionSphere->SetCenter(&Vector3(m_Position.x, m_Position.y + 7.0f, m_Position.z));
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

