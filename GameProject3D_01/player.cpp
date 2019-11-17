
// インクルード ////////////////////////////////////
#include "game_objects_all.h"
#include "scene.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"
#include "skinmesh_animation.h"

#define Glavity (-0.098f)
#define Mass	(3.0f)

static int f = 0;
static double t = 0.0;

void CPlayer::Init()
{
	// モデルの初期化
	m_pModel = new CSkinModel();
	m_pModel->Load("asset/model/SAKURA1.fbx", 0.005f);
	//m_pModel->SetAnimationIndex(0);

	m_Texture = new CTexture();
	m_Texture->LoadSTB("asset/black.png");

	// トランスフォーム初期化
	m_Position = Vector3(0.0f, 20.0f, 0.0f);
	m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
	m_Scale = Vector3(1.0f, 1.0f, 1.0f);

	// 前ベクトル初期化
	m_DirVec.front = { 0.0f, 0.0f, -1.0f };
	m_DirVec.front.Normalize();

	// 右ベクトル初期化
	m_DirVec.up = { 0.0f, 1.0f, 0.0f };
	m_DirVec.right = m_DirVec.up.VCross(m_DirVec.front);
	m_DirVec.right.Normalize();

	// 上ベクトル初期化
	m_DirVec.up = m_DirVec.front.VCross(m_DirVec.right);
	m_DirVec.up.Normalize();

	// コリジョンの初期化
	m_CollisionSphere = new CCollisionSphere;
	m_CollisionSphere->SetRadius(0.7f);

	m_CollisionOBB = new CCollisionOBB(m_Position, m_DirVec, Vector3(0.5f, 0.5f, 0.5f));

	// ダメージ関連ステータスの初期化
	m_DamageManager = new CDamage(100, 15);
	m_DamageManager->GetCollisionSphere()->SetRadius(0.5f);
}

void CPlayer::Uninit()
{
	delete m_DamageManager;
	delete m_CollisionOBB;
	delete m_CollisionSphere;

	m_Texture->Unload();
	delete m_Texture;

	m_pModel->Unload();
	delete m_pModel;
}

void CPlayer::Update()
{
	XMFLOAT3 prevPos = m_Position;
	XMFLOAT3 cameraFront = m_pCamera->GetDir3Vector()->front;
	XMFLOAT3 cameraRight = m_pCamera->GetDir3Vector()->right;

	if (CInput::GetKeyPress(VK_RSHIFT))
	{
		if (CInput::GetKeyPress('W')) {
			m_Position.x += cameraFront.x * 0.05f;
			m_Position.z += cameraFront.z * 0.05f;
		}
		if (CInput::GetKeyPress('A')) {
			m_Position.x -= cameraRight.x * 0.05f;
			m_Position.z -= cameraRight.z * 0.05f;
		}
		if (CInput::GetKeyPress('S')) {
			m_Position.x -= cameraFront.x * 0.05f;
			m_Position.z -= cameraFront.z * 0.05f;
		}
		if (CInput::GetKeyPress('D')) {
			m_Position.x += cameraRight.x * 0.05f;
			m_Position.z += cameraRight.z * 0.05f;
		}
	}

	if (CInput::GetKeyTrigger(VK_SPACE)) {

		CSound::Play(SOUND_LABEL_SE_ATTACK);

		std::vector<CEnemy*> enemys;
		enemys = CManager::GetScene()->GetGameObjects<CEnemy>(CManager::E_3D);

		// 敵と攻撃範囲の当たり判定
		for (CEnemy* enemy : enemys) {
			if (enemy != nullptr) {

				// 当たり判定
				if (CJudgeCollision3D::Collision3D_Spher_Spher(enemy->GetCollisionSphere(), m_DamageManager->GetCollisionSphere())) {

					CSound::Play(SOUND_LABEL_SE_HIT);
					// ダメ―ジ判定
					m_DamageManager->DoDamage(enemy->GetDamageManager());
				}
			}
		}
	}

	XMFLOAT3 FrontDir;
	FrontDir.x = m_Position.x - prevPos.x;
	FrontDir.y = 0.0f;
	FrontDir.z = m_Position.z - prevPos.z;

	if ((fabs(FrontDir.x) > 0.001f) || (fabs(FrontDir.z) > 0.001f)) {
		m_Rotation.y = atan2f(FrontDir.x, FrontDir.z);
	}

	XMMATRIX rotationMtx;
	rotationMtx = XMMatrixRotationAxis(m_DirVec.up, m_Rotation.y);
	m_DirVec.right = XMVector3TransformNormal(m_DirVec.right, rotationMtx);
	m_DirVec.right = XMVector3Normalize(m_DirVec.right);
	m_DirVec.front = XMVector3TransformNormal(m_DirVec.front, rotationMtx);
	m_DirVec.front = XMVector3Normalize(m_DirVec.front);

	//if (m_Position.x + m_CollisionSphere->GetRadius() > 15.0) m_Position.x = 15.0f - m_CollisionSphere->GetRadius();
	//if (m_Position.x - m_CollisionSphere->GetRadius() < -15.0) m_Position.x = -15.0f + m_CollisionSphere->GetRadius();
	//if (m_Position.z + m_CollisionSphere->GetRadius() > 15.0) m_Position.z = 15.0f - m_CollisionSphere->GetRadius();
	//if (m_Position.z - m_CollisionSphere->GetRadius() < -15.0) m_Position.z = -15.0f + m_CollisionSphere->GetRadius();

	// コリジョン位置の更新
	m_CollisionSphere->SetCenter(&m_Position);
	m_DamageManager->GetCollisionSphere()->SetCenter(&Vector3(m_Position.x + FrontDir.x * 2, m_Position.y + FrontDir.y * 2, m_Position.z + FrontDir.z * 2));

	m_CollisionOBB->SetStatus(&m_Position, &m_DirVec, &Vector3(1.0f, 1.0f, 1.0f));

	// 重力
	float move_y = Glavity * Mass * t * t * 0.5f;
	m_Position.y += move_y;

	// 地形との衝突判定
	bool landing = IsLanding();
	if (landing) {
		t = 0.0f;
	}
	else {
		t += DELTA_TIME;
	}

	// モデル更新
	m_pModel->Animation(f++);

	// 最下層
	if (m_Position.y < -20.0f) { m_Position.y = -20.0f; }
}

void CPlayer::Draw()
{
	// カリング
	//CCamera* camera;
	//camera = CManager::GetScene()->GetGameObject<CCamera>(CManager::E_Camera);

	//if (camera->GetVisivility(&m_Position) == false) {
	//	return;
	//}

	//XMFLOAT3 pos = *m_pModel->GetNodePosition("LeftFoot");

	DrawGUI();

	// マトリクス設定
	XMMATRIX world;
	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	world *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	// モデル描画
	//CRenderer::SetTexture(m_Texture);
	m_pModel->Draw(&world);

	// デバッググリッドセット
	XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
	CDebugPrimitive::DebugPrimitive_BatchCirecleDraw(m_CollisionSphere, &color);
	color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	CDebugPrimitive::DebugPrimitive_BatchCirecleDraw(m_DamageManager->GetCollisionSphere(), &color);
	color = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
	CDebugPrimitive::DebugPrimitive_BatchCubeDraw(m_CollisionOBB, &color);
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

		m_DamageManager->DebugDraw();

		ImGui::Columns(3, "Player");

		//＜:: Status ::＞
		if(ImGui::CollapsingHeader("Status"))
		{
			int id = ImGui::GetColumnIndex();
			float width = ImGui::GetColumnWidth(id);

			ImGuiID Window_Status_Id = ImGui::GetID("Status");

			ImGui::BeginChildFrame(Window_Status_Id, ImVec2(width, 100));
			ImGui::InputFloat3("Position", (float*)&m_Position);
			ImGui::Text("PosX = %.1f", m_Position.x);
			ImGui::Text("PosY = %.1f", m_Position.y);
			ImGui::Text("PosZ = %.1f", m_Position.z);
			ImGui::EndChildFrame();
		}

		ImGui::NextColumn();

		//＜:: Animation ::＞
		if (ImGui::CollapsingHeader("Animation"))
		{
			int id = ImGui::GetColumnIndex();
			float width = ImGui::GetColumnWidth(id);

			ImGuiID Window_Animation_Id = ImGui::GetID("Animation");

			ImGui::BeginChildFrame(Window_Animation_Id, ImVec2(width, 100));
			{
				if (ImGui::Button("<")) { m_pModel->SetAnimIdNext(false); }ImGui::SameLine();
				ImGui::Text(m_pModel->GetCurrentAnimName()); ImGui::SameLine();
				if (ImGui::Button(">")) { m_pModel->SetAnimIdNext(true); }

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

bool CPlayer::IsLanding()
{
	// 地面とのコリジョン
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

void CPlayer::SetCamera(CCamera* pCamera)
{
	m_pCamera = pCamera;
}


