
// �C���N���[�h ////////////////////////////////////
#include "game_objects_all.h"
#include "scene.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"
#include "shader_all.h"
#include "skinmesh_animation.h"
#include "state_player_idle.h"
#include "state_player_dodge.h"

#define Glavity (-0.098f)
#define Mass	(10.0f)

void CPlayer::Init()
{
	// ���f���̏�����
	m_pModel = new CSkinModel();
	m_pModel->Load("asset/model/Human.fbx", 0.0023f, "asset/image/white.png", "asset/NodeNameFiles/player_Node.txt" );

	// ���
	m_pState = new CStatePlayerIdle(this);

	// �e
	m_Shadow = new CPolygon3D();
	m_Shadow->Init(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.7f, 1.0f, 1.7f), Vector3(0.0f, 0.0f, 0.0f), "asset/image/shadow.png");

	// �g�����X�t�H�[��������
	m_Position = Vector3(70.0f, 0.0f, -100.0f);
	m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
	m_Scale = Vector3(1.0f, 1.0f, 1.0f);

	// Front_Up_Right�x�N�g��������
	m_DirVec.SetFrontUpRight(Vector3(0.0f, 0.0f, 1.0f));

	// �R���W�����̏�����
	m_CollisionSphere = new CCollisionSphere(m_Position, 1.1f);
	m_CollisionOBB = new CCollisionOBB(m_Position, m_DirVec, Vector3(0.5f, 0.5f, 0.5f));

	// �_���[�W�֘A�X�e�[�^�X�̏�����
	m_DamageManager = new CDamage(100, 15);
	m_DamageManager->GetCollisionSphere()->SetRadius(0.2f);

	m_MoveDistance = Vector3(0.0f, 0.0f, 0.0f);
	m_BonePosition = Vector3(0.0f, 0.0f, 0.0f);
	m_MoveSpeed = m_DefaultSpeed;
	m_IsCollision = false;
}

void CPlayer::Uninit()
{
	delete m_DamageManager;
	delete m_CollisionOBB;
	delete m_CollisionSphere;

	m_Shadow->Uninit();
	delete m_Shadow;

	delete m_pState;

	m_pModel->Unload();
	delete m_pModel;
}

void CPlayer::Update()
{
	// �ړ�
	Move();

	// �s��
	Action();

	// ���f���X�V
	m_pModel->update(1);
}

void CPlayer::Draw()
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


void CPlayer::DrawCollisionGrid()
{
	// �f�o�b�O�O���b�h�Z�b�g
	XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
	CDebugPrimitive::DebugPrimitive_BatchCirecleDraw(m_CollisionSphere, &color);

	color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	CDebugPrimitive::DebugPrimitive_BatchCirecleDraw(m_DamageManager->GetCollisionSphere(), &color);


	CCollisionSphere vector_sphere;
	vector_sphere.SetRadius(0.3f);
	vector_sphere.SetCenter(
		&Vector3(
			m_Position.x + m_DirVec.front.x,
			m_Position.y + m_DirVec.front.y + 1.5f,
			m_Position.z + m_DirVec.front.z
		)
	);

	// OBB
	color = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
	CDebugPrimitive::DebugPrimitive_BatchCubeDraw(m_CollisionOBB, &color);


	////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////
	// OBB
	CCollisionOBB obbCol;
	const float scale = 0.3f;

	// vFront
	color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	obbCol.SetStatus(
		&Vector3(m_Position.x + m_DirVec.up.x + m_DirVec.front.x * scale, m_Position.y + m_DirVec.up.y + 1.5f + m_DirVec.front.y * scale, m_Position.z + m_DirVec.up.z + m_DirVec.front.z * scale),
		&Vector3X3(m_DirVec.right, m_DirVec.up, m_DirVec.front),
		&Vector3(0.05f, 0.05f, scale)
	);
	CDebugPrimitive::DebugPrimitive_BatchCubeDraw(&obbCol, &color);

	// vUp
	color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	obbCol.SetStatus(
		&Vector3(m_Position.x + m_DirVec.up.x + m_DirVec.up.x * scale, m_Position.y + m_DirVec.up.y + 1.5f + m_DirVec.up.y * scale, m_Position.z + m_DirVec.up.z + m_DirVec.up.z * scale),
		&Vector3X3(m_DirVec.right, m_DirVec.up, m_DirVec.front),
		&Vector3(0.05f, scale, 0.05f)
	);
	CDebugPrimitive::DebugPrimitive_BatchCubeDraw(&obbCol, &color);

	// vRight
	color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	obbCol.SetStatus(
		&Vector3(m_Position.x + m_DirVec.up.x + m_DirVec.right.x * scale, m_Position.y + m_DirVec.up.y + 1.5f + m_DirVec.right.y * scale, m_Position.z + m_DirVec.up.z + m_DirVec.right.z * scale),
		&Vector3X3(m_DirVec.right, m_DirVec.up, m_DirVec.front),
		&Vector3(scale, 0.05f, 0.05f)
	);
	CDebugPrimitive::DebugPrimitive_BatchCubeDraw(&obbCol, &color);
	////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////


	// �e�X�g�R���W����OBB
	obbCol.SetStatus(&Vector3(0.0f, 5.0f, 0.0f), &Vector3X3(Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f)), &Vector3(1.0f, 10.0f, 1.0f));
	color = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
	CDebugPrimitive::DebugPrimitive_BatchCubeDraw(&obbCol, &color);
}

void CPlayer:: DrawGUI()
{
	ImGui::Begin("System");
	ImGuiID Window_System_Id = ImGui::GetID("System");
	ImGui::GetItemRectSize();
	ImVec2 size = ImGui::GetWindowSize();

	if (ImGui::CollapsingHeader("Player"))
	{
		ImGuiID Window_Player_Id = ImGui::GetID("Player");

		ImVec2 s = ImGui::GetWindowSize();
		ImGui::InputFloat3("BonePosition", (float*)&m_BonePosition);

		m_DamageManager->DebugDraw();

		ImGui::Columns(3, "Player");

		//��:: Status ::��
		if(ImGui::CollapsingHeader("Status"))
		{
			int id = ImGui::GetColumnIndex();
			float width = ImGui::GetColumnWidth(id);

			ImGuiID Window_Status_Id = ImGui::GetID("Status");

			ImGui::BeginChildFrame(Window_Status_Id, ImVec2(width, 100));
			ImGui::InputFloat3("Position", (float*)&m_Position);
			ImGui::InputFloat3("Rotation", (float*)&m_Rotation);
			ImGui::Text(m_IsCollision ? "Collision" :"Through");
			ImGui::EndChildFrame();
		}

		ImGui::NextColumn();

		//��:: Animation ::��
		if (ImGui::CollapsingHeader("Animation"))
		{
			int id = ImGui::GetColumnIndex();
			float width = ImGui::GetColumnWidth(id);

			ImGuiID Window_Animation_Id = ImGui::GetID("Animation");

			ImGui::BeginChildFrame(Window_Animation_Id, ImVec2(width, 100));
			{
				ImGui::Text("%d", m_pModel->GetAnimationNum()); ImGui::SameLine();
				if (ImGui::Button("<")) { m_pModel->SetAnimation(false); }ImGui::SameLine();
				ImGui::Text(m_pModel->GetCurrentAnimName()); ImGui::SameLine();
				if (ImGui::Button(">")) { m_pModel->SetAnimation(true); }

				ImGui::SliderFloat("Speed", m_pModel->AnimationSpeed(), 0.01f, 10.0f);

				ImGui::Checkbox("DrawAtLine", m_pModel->DrawAtLine());

				ImGui::Checkbox("Stop", m_pModel->IsStopMotion());

				if(*m_pModel->IsStopMotion())
				{
					ImGui::SameLine();
					ImGui::SliderInt("Frame", m_pModel->MotionFrame(), 0, 100);
				}

			}
			ImGui::EndChildFrame();
		}
	}

	ImGui::End();
}

void CPlayer::Move()
{
	CCamera* camera = CCameraManager::GetCamera();
	if (!camera->GetIsBindAtObject())return;

	// ��Ԃ̍X�V
	m_pState->Update(this);

	// �R���W�����ʒu�̍X�V
	UpdateCollision();

	// �G�Ƃ̃R���W��������
	std::vector<CEnemy*> game_obj = CManager::GetScene()->GetGameObjects<CEnemy>(CManager::E_3D);
	for (CEnemy*  obj : game_obj) {

		// �R���W��������
		std::vector< CCollisionSphere*> collision = obj->GetCollisionSphere();
		for (CCollisionSphere* coll : collision) {
			if (CCollision3DJudge::Collision3D_Spher_Spher(m_CollisionSphere, coll)) {

				float distance = m_CollisionSphere->GetRadius() + coll->GetRadius();
				Vector3 vec = m_CollisionSphere->GetCenter() - coll->GetCenter();
				float length = vec.Length();
				vec.Normalize();
				vec = vec * (distance - length);
				m_Position += vec;
				break;
			}
		}
	}

	// �����x�N�g����]
	Vector3 front = Vector3(0.0f, 0.0f, 1.0f);
	XMMATRIX rotationMtx;
	rotationMtx = XMMatrixRotationY(m_Rotation.y);

	front = XMVector3TransformNormal(front, rotationMtx);
	front.Normalize();
	m_DirVec.front = front;
	m_DirVec.right = -m_DirVec.front.VCross(m_DirVec.up);
	m_DirVec.right.Normalize();

	// �d�͉��Z
	AddGlavity();

	// �e�̍X�V
	m_Shadow->SetPosition(&Vector3(m_Position.x, 0.01f, m_Position.z));
	m_Shadow->Update();
}


void CPlayer::Action()
{
	if (CInput::GetKeyTrigger(VK_SPACE)) {

		//CSound::Play(SOUND_LABEL_SE_ATTACK);

		//ChangeState(new CStatePlayerDodge(this));

		//std::vector<CEnemy*> enemys;
		//enemys = CManager::GetScene()->GetGameObjects<CEnemy>(CManager::E_3D);

		//// �G�ƍU���͈͂̓����蔻��
		//for (CEnemy* enemy : enemys) {
		//	if (enemy != nullptr) {

		//		// �����蔻��
		//		if (CCollision3DJudge::Collision3D_Spher_Spher(enemy->GetCollisionSphere(), m_DamageManager->GetCollisionSphere())) {

		//			CSound::Play(SOUND_LABEL_SE_HIT);
		//			// �_���\�W����
		//			m_DamageManager->DoDamage(enemy->GetDamageManager());
		//		}
		//	}
		//}
	}
}


void CPlayer::UpdateCollision()
{
	// �}�g���N�X�ݒ�
	XMMATRIX world;
	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	world *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	// ����̃{�[���̈ʒu���擾
	m_BonePosition = m_pModel->GetWorldPosition(&world, "mixamorig:LeftHandIndex4_end");
	m_DamageManager->GetCollisionSphere()->SetCenter(&(m_BonePosition));
	m_DamageManager->GetCollisionSphere()->SetRadius(0.2f);
	m_CollisionSphere->SetCenter(&m_pModel->GetWorldPosition(&world, "mixamorig:Hips"));

	// �R���W�����X�V
	Vector3X3 obbColSize;
	obbColSize.front = m_DirVec.front;
	obbColSize.up = m_DirVec.up;
	obbColSize.right = m_DirVec.right;
	m_CollisionOBB->SetStatus(&Vector3(m_Position.x, m_Position.y + 1.0f, m_Position.z), &obbColSize, &Vector3(0.5f, 1.0f, 0.5f));

	// �Փ˔���e�X�g
	CCollisionOBB obbCol;
	obbCol.SetStatus(&Vector3(0.0f, 5.0f, 0.0f), &Vector3X3(Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f)), &Vector3(1.0f, 10.0f, 1.0f));
	m_IsCollision = CCollision3DJudge::Collision3D_OBB_OBB(*m_CollisionOBB, obbCol);
}

void CPlayer::AddGlavity()
{
	static double t = 0.0;

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


bool CPlayer::IsLanding()
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

bool CPlayer::CurrentAnimationFinish(void)
{
	return m_pModel->CurrentAnimationFinish();
}

int CPlayer::GetCurrentAnimFrameNum()
{
	return m_pModel->GetCurrentAnimFrameNum();
}

bool& CPlayer::AnimationBlending(void)
{
	return m_pModel->AnimationBlending();
}

void CPlayer::SetAnimation(const unsigned int _id, const float _startBlendNum)
{
	m_pModel->SetAnimation(_id, _startBlendNum);
}

void CPlayer::SetAnimationSpeed(const float _speed)
{
	m_pModel->SetAnimationSpeed(_speed);
}

void CPlayer::ChangeState(CStatePlayer* pState)
{
	delete m_pState;
	m_pState = pState;
}

