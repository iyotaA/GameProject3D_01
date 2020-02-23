
// �C���N���[�h ////////////////////////////////////
#include "game_objects_all.h"
#include "scene.h"
#include "fade_manager.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"
#include "shader_all.h"
#include "skinmesh_animation.h"
#include "main.h"
#include "state_enemy_idle.h"
#include "state_enemy_died.h"
#include "MathFunc.h"

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
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 1.5f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "B_Head"));			// ��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 2.0f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "B_Pelvis"));			// ��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 2.0f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "B_Hip"));				// �K
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 1.0f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "B_Chin"));				// �{
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 2.5f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "B_Spine"));			// �w��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 1.0f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "B_Spine.002"));		// �w��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 1.0f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "B_Spine.003"));		// �w��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 1.0f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "B_Spine.004"));		// �w��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 1.0f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "B_R_Back_Leg"));	// ���E��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 1.0f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "B_L_Back_Leg"));	// ��덶��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 1.0f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "B_R_Back_Foot"));	// ���E��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 1.0f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "B_L_Back_Foot"));	// ��덶��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 1.0f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "B_R_Front_Leg"));	// �O�E�r
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 1.0f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "B_L_Front_Leg"));	// �O���r
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 0.7f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "B_R_Front_Foot")); // �O�E��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 0.7f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "B_L_Front_Foot")); // �O����
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 1.5f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "B_Tail.001"));		 // �K��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 1.0f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "B_Tail.002"));		 // �K��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 0.8f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "B_Tail.003"));		 // �K��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 0.8f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "B_Tail.004"));		 // �K��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 0.8f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "B_Tail.005"));		 // �K��
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 0.8f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "B_Tail.006"));		 // �K��


	//---   ����R���W�����̏�����   -----------------------------------------------------------------------------
	m_JudgeArea.push_back(new CCollisionSphere(m_Position, 8.0f, XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)));	// �ߋ����U���͈�
	m_JudgeArea.push_back(new CCollisionSphere(m_Position, 4.0f, XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)));	// �ߋ����O���U���͈�
	m_JudgeArea.push_back(new CCollisionSphere(m_Position, 35.0f, XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)));	// ���G�͈�
	m_JudgeArea.push_back(new CCollisionSphere(m_Position, 3.0f, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)));	// �n�`�Ƃ̓����蔻��
	m_JudgeArea.push_back(new CCollisionSphere(m_Position, 3.0f, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)));	// �n�`�Ƃ̓����蔻��


	//---   ���̑������o�̏�����   ----------------------------------------------------------------------------
	m_MoveDistance = Vector3(0.0f, 0.0f, 0.0f);
	m_MoveSpeed = m_DEFAULT_SPEED;
	m_IsCollision = false;
	m_IsPressMovingEntry = false;

	//---   �t���O������ --------------------------------------------------------------------------------------
	m_StateFlags.Damage			= false;
	m_StateFlags.Died					= false;
	m_StateFlags.Scouting			= false;
	m_StateFlags.Attack				= false;
	m_StateFlags.Battle				= false;
	m_StateFlags.InScoutingArea	= false;
	m_StateFlags.InNearArea		= false;
	m_StateFlags.InNearFrontArea= false;
	m_StateFlags.CollisionTerrian	= false;

	m_Status.Life = 300;
	m_Status.Attack = 0;
}

void CEnemy::Uninit()
{
	m_JudgeArea.clear();
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

	// �R���W��������
	CheckCollision();

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

	// �R���W��������
	DrawCollisionGrid();
}

void CEnemy::CheckCollision()
{
	// �v���C���[�Q�b�g
	CPlayer* player = CManager::GetScene()->GetGameObject<CPlayer>(CManager::LAYER_OBJECT);
	if (player->Died() || Died()) {	// �v���C���[���́A���������S���Ă�����X�V���Ȃ�
		m_StateFlags.InNearArea = false;
		m_StateFlags.InScoutingArea = false;

		if (m_StateFlags.InScoutingArea) {
			ChangeState(new CStateEnemyIdle(this));
		}
		return;
	}

	// �͈͓����`�F�b�N�iplayer�̓��Ƃ̔���j
	m_StateFlags.InNearArea		 = CCollision3DJudge::Collision3D_Spher_Spher(m_JudgeArea[0], player->GetCollisionSphere(1)->GetSphere());
	m_StateFlags.InNearFrontArea = CCollision3DJudge::Collision3D_Spher_Spher(m_JudgeArea[1], player->GetCollisionSphere(1)->GetSphere());
	m_StateFlags.InScoutingArea   = CCollision3DJudge::Collision3D_Spher_Spher(m_JudgeArea[2], player->GetCollisionSphere(1)->GetSphere());

	// �퓬��Ԃ���Ȃ���?
	if (!m_StateFlags.Battle) {
		if (m_StateFlags.InScoutingArea) {
			m_StateFlags.Battle = m_StateFlags.InScoutingArea;
			m_JudgeArea[2]->SetRadius(50.0f);
			CSound::StopSound(SOUND_LABEL_BGM_ENVIRONMENT);
			CSound::Play(SOUND_LABEL_BGM_GAME);
		}
	}
	else {
		if (!m_StateFlags.InScoutingArea) {
			m_StateFlags.Battle = m_StateFlags.InScoutingArea;
			m_JudgeArea[2]->SetRadius(35.0f);
			CSound::StopSound(SOUND_LABEL_BGM_GAME);
			CSound::Play(SOUND_LABEL_BGM_ENVIRONMENT);
		}
	}
}

void CEnemy::DrawCollisionGrid()
{
	// �R���W�����\���i���F�j
	for (CCollisionWithBone* coll : m_CollisionBody) {
		CDebugPrimitive::DebugPrimitive_BatchCirecleDraw(coll->GetSphere());
	}

	// ���G�͈͕`��i�j
	for (CCollisionSphere* coll : m_JudgeArea) {
		CDebugPrimitive::DebugPrimitive_BatchCirecleDraw(coll);
	}
}

void CEnemy::DrawGUI()
{
	ImGui::Begin("System");
	ImGuiID Window_System_Id = ImGui::GetID("System");
	ImGui::GetItemRectSize();
	ImVec2 size = ImGui::GetWindowSize();

	if (ImGui::CollapsingHeader("Enemy"))
	{
		//��:: Status ::��
		{
			ImGuiID Window_Status_Id = ImGui::GetID("Status");

			ImGui::BeginChildFrame(Window_Status_Id, ImVec2(size.x, 100));
			ImGui::InputFloat3("Position", (float*)&m_Position);
			ImGui::InputFloat3("Rotation", (float*)&m_Rotation);
			ImGui::InputInt("HP", (int*)&m_Status.Life);
			ImGui::Checkbox("Stop", m_pModel->IsStopMotion());
			ImGui::EndChildFrame();
		}
	}

	ImGui::End();
}

void CEnemy::Move()
{
	// �d�͉��Z
	AddGlavity();


	// ��]�p�x���K��(0 �` 360)
	if (m_Rotation.y >= 360.0f * DEGREE_TO_RADIAN) {
		m_Rotation.y = 0.0f;
	}


	// �����x�N�g����]
	Vector3 front = Vector3(0.0f, 0.0f, 1.0f);
	XMMATRIX rotationMtx;
	rotationMtx = XMMatrixRotationY(m_Rotation.y);


	// �O�����x�N�g���X�V
	front = XMVector3TransformNormal(front, rotationMtx);
	front.Normalize();
	m_DirVec.front = front;
	m_DirVec.right = -m_DirVec.front.VCross(m_DirVec.up);
	m_DirVec.right.Normalize();


	// �e�̍X�V�i���̂̈ʒu�Ɉړ��j
	Vector3 pos = m_CollisionBody[1]->GetSphere()->GetCenter();
	m_Shadow->SetPosition(&Vector3(pos.x, 0.01f, pos.z));
	m_Shadow->SetRotation(&Vector3(m_Rotation.x, m_Rotation.y, m_Rotation.z));
	m_Shadow->Update();


	// �R���W�����ʒu�X�V
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

	// �͈͂𒆐S���W�ɃZ�b�g����
	for (CCollisionSphere* coll : m_JudgeArea) {
		Vector3 pos = m_CollisionBody[2]->GetSphere()->GetCenter();	// ���̈ʒu�擾
		pos.y = 0.0f;
		coll->SetCenter(&pos);
	}

	{
		Vector3 pos = m_CollisionBody[4]->GetSphere()->GetCenter();
		pos += m_DirVec.front * 2;
		pos.y = 0.0f;
		m_JudgeArea[1]->SetCenter(&pos);	// �ߋ����O���U���͈�
	}
	{
		Vector3 pos_hip = m_CollisionBody[16]->GetSphere()->GetCenter();	// �K���̕t�����̈ʒu�擾
		m_JudgeArea[m_JudgeArea.size() - 2]->SetCenter(&Vector3(pos_hip.x, 0.0f, pos_hip.z));  // �n�`�Ƃ̓����蔻��
	}
	{
		Vector3 pos_spine = m_CollisionBody[4]->GetSphere()->GetCenter();	// ��̕t�����̈ʒu�擾
		m_JudgeArea[m_JudgeArea.size() - 1]->SetCenter(&Vector3(pos_spine.x, 0.0f, pos_spine.z));  // �n�`�Ƃ̓����蔻��
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
	// �n�ʂƂ̃R���W����
	CTerrain* pTerrain = CManager::GetScene()->GetGameObject<CTerrain>(CManager::LAYER_BACKGROUND);

	Vector3 push_Vec;
	if (pTerrain->GetCollision(m_JudgeArea[m_JudgeArea.size() - 2], push_Vec)) {
		m_Position += Vector3(push_Vec.x, 0.0f, push_Vec.z);
		m_StateFlags.CollisionTerrian = true;
	}
	else if (pTerrain->GetCollision(m_JudgeArea[m_JudgeArea.size() - 1], push_Vec)) {
		m_Position += Vector3(push_Vec.x, 0.0f, push_Vec.z);
		m_StateFlags.CollisionTerrian = true;
	}
	else {
		m_StateFlags.CollisionTerrian = false;
	}
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

void CEnemy::SetMotionStop(const bool _stop)
{
	m_pModel->StopMotion(_stop);
}

void CEnemy::ChangeState(CStateEnemy* pState)
{
	delete m_pState;
	m_pState = pState;
}

