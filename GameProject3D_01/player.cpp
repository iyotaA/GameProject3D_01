
// インクルード ////////////////////////////////////
#include "game_objects_all.h"
#include "image.h"
#include "scene.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"
#include "shader_all.h"
#include "skinmesh_animation.h"
#include "state_player_idle.h"
#include "state_player_block.h"
#include "state_player_died.h"
#include "MathFunc.h"
#include "recovery_drag.h"

#define Glavity (-0.098f)
#define Mass	(10.0f)

void CPlayer::Init()
{
	// モデルの初期化
	m_pModel = new CSkinModel();
	m_pModel->Load("asset/model/human.fbx", 0.0023f, "asset/image/white.png", "asset/NodeNameFiles/player_Node.txt" );

	// 武器モデルの初期化
	m_pWeapon = new CSkinModel();
	m_pWeapon->Load("asset/model/sword.fbx", 40.0f, "asset/image/dragon.png", NULL);

	// 状態
	m_pState = new CStatePlayerIdle(this);

	// 影
	m_Shadow = new CPolygon3D();
	m_Shadow->Init(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.7f, 1.0f, 1.7f), Vector3(0.0f, 0.0f, 0.0f), "asset/image/shadow.png");

	// トランスフォーム初期化
	m_Position = m_SPAWN_POSITION;
	m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
	m_Scale = Vector3(1.0f, 1.0f, 1.0f);

	// Front_Up_Rightベクトル初期化
	m_DirVec.SetFrontUpRight(Vector3(0.0f, 0.0f, 1.0f));

	// コリジョンの初期化
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 0.3f  , XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)),  "mixamorig:Head"));			// 頭
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 0.5f  , XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)),  "mixamorig:Hips"));			// 胴
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 0.15f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "mixamorig:RightFoot"));	// 右足
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 0.15f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "mixamorig:LeftFoot"));		// 左足
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 0.15f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "mixamorig:RightHand"));	// 右手
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 0.15f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "mixamorig:LeftHand"));	// 左手
	m_CollisionBody.push_back(new CCollisionWithBone(new CCollisionSphere(Vector3(), 0.5f, XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)), "RootNode"));						// ルート


	// 剣のコリジョンの初期化
	m_CollisionWeapon.push_back(new CCollisionSphere(m_Position, 0.2f, XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f))); // 剣根
	m_CollisionWeapon.push_back(new CCollisionSphere(m_Position, 0.2f, XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)));
	m_CollisionWeapon.push_back(new CCollisionSphere(m_Position, 0.2f, XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)));
	m_CollisionWeapon.push_back(new CCollisionSphere(m_Position, 0.2f, XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)));
	m_CollisionWeapon.push_back(new CCollisionSphere(m_Position, 0.2f, XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)));
	m_CollisionWeapon.push_back(new CCollisionSphere(m_Position, 0.2f, XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f))); // 剣先

	m_MoveDistance = Vector3(0.0f, 0.0f, 0.0f);
	m_MoveSpeed = m_DEFAULT_SPEED;

	m_WeaponState = SWORD_STATE_SHEATHE;
	m_WeaponBoneName.push_back("B_Weapon_Waist");
	m_WeaponBoneName.push_back("B_Weapon");

	m_Status.DeadTimes = 0;
	m_Status.Life = 100;
	m_Status.Stamina = 100;
	m_Status.AttackValue = 10.0f;

	m_FrameCounter = 30;

	m_Collision = false;
	m_StateFlags.Idle = false;
	m_StateFlags.Damage = false;
	m_StateFlags.Dodge = false;
	m_StateFlags.Attack = false;
	m_StateFlags.Block = false;
	m_StateFlags.Dash = false;
	m_StateFlags.Move = false;
	m_StateFlags.Died = false;
	m_StateFlags.StaminaEmpty = false;


	m_Item = new CRecoveryDrag(10);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//		UIオブジェクトの初期化
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// ダメージ値
	m_Number = new CNumber("asset/image/user_interface/number.png");
	m_Number->SetNum(100);
	m_Number->SetSize(XMFLOAT2(25.0f, 25.0f));
	m_Number->SetPosition(XMFLOAT2(40.0f, 200.0f));
	m_Number->SetColor(XMFLOAT4(1.0f, 0.5f, 0.0f, 0.0f));
	CUserInterfaceManager::AddUI(m_Number, CUserInterfaceManager::LAYER_1);// UIに追加する

	// ライフゲージ
	m_UIAccesser.push_back(new CImage("asset/image/user_interface/guage.png"));
	m_UIAccesser[0]->SetSize(XMFLOAT2(600.0f, 10.0f));
	m_UIAccesser[0]->SetPosition(XMFLOAT2(400.0f, 20.0f));
	m_UIAccesser[0]->SetColor(XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
	m_UIAccesser[0]->SetUV(UV(0.0f, 0.0f, 0.5f, 1.0f));

	// スタミナゲージ
	m_UIAccesser.push_back(new CImage("asset/image/user_interface/guage.png"));
	m_UIAccesser[1]->SetSize(XMFLOAT2(560.0f, 10.0f));
	m_UIAccesser[1]->SetPosition(XMFLOAT2(420.0f, 40.0f));
	m_UIAccesser[1]->SetColor(XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
	m_UIAccesser[1]->SetUV(UV(0.0f, 0.0f, 0.5f, 1.0f));

	// UIに追加する
	for (CImage* ui : m_UIAccesser) {
		CUserInterfaceManager::AddUI(ui, CUserInterfaceManager::LAYER_0);
	}
}

void CPlayer::Uninit()
{
	m_WeaponBoneName.clear();
	m_CollisionWeapon.clear();
	m_CollisionBody.clear();

	m_Shadow->Uninit();
	delete m_Shadow;

	delete m_Number;

	delete m_pState;

	m_pWeapon->Unload();
	delete m_pWeapon;

	m_pModel->Unload();
	delete m_pModel;
}

void CPlayer::Update()
{
	// 移動
	Move();

	float u;
	u = (m_Status.Life > 0) ? 0.5f - m_Status.Life / 100.0f * 0.5f : 0.5f;
	m_UIAccesser[0]->SetUV(UV(u, 0.0f, u + 0.5f, 1.0f));
	u = (m_Status.Stamina > 0) ? 0.5f - m_Status.Stamina / 100.0f * 0.5f : 0.5f;
	m_UIAccesser[1]->SetUV(UV(u, 0.0f, u + 0.5f, 1.0f));

	// ダメージ値を徐々に消す
	float alpha = 1.0f - m_FrameCounter / 50.0f;
	if (alpha <= 0.0f) { alpha = 0.0f; }
	m_Number->SetColor(XMFLOAT4(1.0f, 0.5f, 0.0f, alpha));

	// モデル更新
	m_pModel->update(1);

	m_FrameCounter++;
}

void CPlayer::Draw()
{
	// マトリクス設定
	XMMATRIX world;
	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	world *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	// モデル描画
	m_pModel->Draw(&world);

	// 武器描画
	m_pWeapon->Draw(m_pModel->GetBoneMatrix(&world, m_WeaponBoneName[m_WeaponState].c_str()));

	// 影描画
	m_Shadow->Draw();

	// コリジョン描画
	DrawCollisionGrid();
}


void CPlayer::DrawCollisionGrid()
{
	XMFLOAT4 color;

	// 体のコリジョン表示
	color = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
	for (CCollisionWithBone* coll : m_CollisionBody) {
		CDebugPrimitive::DebugPrimitive_BatchCirecleDraw(coll->GetSphere());
	}

	// 剣のコリジョン表示
	color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	for (CCollisionSphere* coll : m_CollisionWeapon) {
		CDebugPrimitive::DebugPrimitive_BatchCirecleDraw(coll);
	}

	////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////
	// OBB
	CCollisionOBB obbCol;
	const float scale = 0.3f;

	// vFront
	color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	obbCol.SetStatus(&Vector3(m_Position.x + m_DirVec.up.x + m_DirVec.front.x * scale, m_Position.y + m_DirVec.up.y + 1.5f + m_DirVec.front.y * scale, m_Position.z + m_DirVec.up.z + m_DirVec.front.z * scale),&Vector3X3(m_DirVec.right, m_DirVec.up, m_DirVec.front),&Vector3(0.05f, 0.05f, scale), & color);
	CDebugPrimitive::DebugPrimitive_BatchCubeDraw(&obbCol);

	// vUp
	color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	obbCol.SetStatus( &Vector3(m_Position.x + m_DirVec.up.x + m_DirVec.up.x * scale, m_Position.y + m_DirVec.up.y + 1.5f + m_DirVec.up.y * scale, m_Position.z + m_DirVec.up.z + m_DirVec.up.z * scale), &Vector3X3(m_DirVec.right, m_DirVec.up, m_DirVec.front),	&Vector3(0.05f, scale, 0.05f), &color);
	CDebugPrimitive::DebugPrimitive_BatchCubeDraw(&obbCol);

	// vRight
	color = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	obbCol.SetStatus(&Vector3(m_Position.x + m_DirVec.up.x + m_DirVec.right.x * scale, m_Position.y + m_DirVec.up.y + 1.5f + m_DirVec.right.y * scale, m_Position.z + m_DirVec.up.z + m_DirVec.right.z * scale), &Vector3X3(m_DirVec.right, m_DirVec.up, m_DirVec.front), &Vector3(scale, 0.05f, 0.05f), &color);
	CDebugPrimitive::DebugPrimitive_BatchCubeDraw(&obbCol);
	////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////


	// テストコリジョンOBB
	color = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
	obbCol.SetStatus(&Vector3(0.0f, 5.0f, 0.0f), &Vector3X3(Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f)), &Vector3(1.0f, 10.0f, 1.0f), &color);
	CDebugPrimitive::DebugPrimitive_BatchCubeDraw(&obbCol);
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

		//＜:: Status ::＞
		{
			ImGuiID Window_Status_Id = ImGui::GetID("Status");

			ImGui::BeginChildFrame(Window_Status_Id, ImVec2(s.x, 100));
			ImGui::InputFloat3("Position", (float*)&m_Position);
			ImGui::InputFloat3("Rotation", (float*)&m_Rotation);
			ImGui::Text(m_Collision ? "Collision" : "Through");
			ImGui::EndChildFrame();
		}

		//＜:: Animation ::＞
		{
			ImGuiID Window_Animation_Id = ImGui::GetID("Animation");

			ImGui::BeginChildFrame(Window_Animation_Id, ImVec2(s.x, 120));
			{
				ImGui::Text("AnimCount : %d", m_pModel->GetAnimationNum());
				if (ImGui::Button("<")) { m_pModel->SetAnimation(false); }ImGui::SameLine();
				ImGui::Text(m_pModel->GetCurrentAnimName()); ImGui::SameLine();
				if (ImGui::Button(">")) { m_pModel->SetAnimation(true); }

				ImGui::SliderFloat("Speed", m_pModel->AnimationSpeed(), 0.01f, 10.0f);

				ImGui::Checkbox("DrawAtLine", m_pModel->DrawAtLine());

				ImGui::Checkbox("Stop", m_pModel->IsStopMotion());

				if (*m_pModel->IsStopMotion())
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
	Vector3 prevPos = m_Position;

	// 状態の更新
	m_pState->Update(this);

	// コリジョン位置の更新
	UpdateCollision();

	// 敵とのコリジョン判定
	CheckCollision_Enemy();

	// 方向ベクトル回転
	Vector3 front = Vector3(0.0f, 0.0f, 1.0f);
	XMMATRIX rotationMtx;
	rotationMtx = XMMatrixRotationY(m_Rotation.y);

	front = XMVector3TransformNormal(front, rotationMtx);
	front.Normalize();
	m_DirVec.front = front;
	m_DirVec.right = -m_DirVec.front.VCross(m_DirVec.up);
	m_DirVec.right.Normalize();

	// 重力加算
	AddGlavity();

	// 影の更新(胴の位置に移動)
	Vector3 pos = m_CollisionBody[1]->GetSphere()->GetCenter();
	m_Shadow->SetPosition(&Vector3(pos.x, 0.001f, pos.z));
	m_Shadow->Update();

	// スタミナ消費処理
	if (m_StateFlags.Dash) {
		(m_Status.Stamina > 0) ? m_Status.Stamina -= 0.15f : m_StateFlags.StaminaEmpty = true;
	}
	else {
		(m_Status.Stamina < 100 && !m_StateFlags.Dodge) ? m_Status.Stamina += 0.2f : m_StateFlags.StaminaEmpty = false;
	}
}

void CPlayer::CheckCollision_Enemy()
{
	//==============================================
	// 敵とのコリジョン判定
	//==============================================
	if (!m_StateFlags.Dodge && !m_StateFlags.Damage) {
		std::vector<CEnemy*> game_obj = CManager::GetScene()->GetGameObjects<CEnemy>(CManager::LAYER_OBJECT);
		for (CEnemy* obj : game_obj) {

			//==============================================
			//  コリジョン判定
			//==============================================
			std::vector< CCollisionWithBone*> collision = obj->GetCollisionSphere();
			for (CCollisionWithBone* enemyColl : collision) {
				if (m_StateFlags.Damage)break;

				//==============================================
				// 体のコリジョン判定（敵が死亡していなければ実行）
				//==============================================
				if (!obj->Died()) {
					for (CCollisionWithBone* playerColl : m_CollisionBody) {
						if (CCollision3DJudge::Collision3D_Spher_Spher(playerColl->GetSphere(), enemyColl->GetSphere())) {

							float distance = playerColl->GetSphere()->GetRadius() + enemyColl->GetSphere()->GetRadius();
							Vector3 vec = playerColl->GetSphere()->GetCenter() - enemyColl->GetSphere()->GetCenter();
							float length = vec.Length();
							vec.Normalize();
							vec = vec * (distance - length);
							m_Position += vec;

							// 敵が攻撃状態ならダメージ
							if (obj->Attacked()) {
								m_StateFlags.Damage = true;
							}
							break;
						}
					}
				}
				//==============================================


				//==============================================
				// 武器のコリジョン判定
				//==============================================
				// 攻撃ステートの時だけ以下実行
				if (!m_StateFlags.Attack)continue;

				for (CCollisionSphere* coll_weapon : m_CollisionWeapon) {

					if (CCollision3DJudge::Collision3D_Spher_Spher(coll_weapon, enemyColl->GetSphere())) {
						// カメラゲット
						CCamera* camera = CCameraManager::GetCamera();
						camera->SetShake();

						// 効果音再生
						CSound::Play(SOUND_LABEL_SE_ATTACK_LARGE);
						m_StateFlags.Attack = false;
						m_Number->SetPosition(GetScreenPos(coll_weapon->GetCenter()));
						m_Number->SetNum(m_Status.AttackValue);
						m_Number->SetColor(XMFLOAT4(1.0f, 0.5f, 0.0f, 1.0f));
						m_FrameCounter = 0;

						// ダメージを与える
						obj->DealDamage(m_Status.AttackValue);
						break;
					}
				}
				//==============================================
			}
		}
	}
}

void CPlayer::UpdateCollision()
{
	// マトリクス設定
	XMMATRIX world;
	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	world *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);


	// 特定のボーンの位置を取得
	for (CCollisionWithBone* coll : m_CollisionBody) {

		coll->GetSphere()->SetCenter(&m_pModel->GetWorldPosition(&world, coll->GetBoneName()));
	}


	// 武器コリジョン位置更新（剣の根元から剣先まで球型コリジョンを５個）
	Vector3 weapon_Root = m_pModel->GetWorldPosition(&world, "B_Weapon");
	Vector3 weapon_Vector = m_pModel->GetWorldPosition(&world, "B_Weapon_Top") - weapon_Root;
	float length = 0.0f;
	for (CCollisionSphere* coll : m_CollisionWeapon) {
		length += 1.2f / m_CollisionWeapon.size();
		coll->SetCenter(&(weapon_Root + weapon_Vector * length));
	}
}

void CPlayer::AddGlavity()
{
	static double t = 0.0;

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

	// 最下層（これ以下に下がらないように）
	if (m_Position.y < -20.0f) { m_Position.y = -20.0f; }
}


bool CPlayer::IsLanding()
{
	// 地面とのコリジョン
	CTerrain* pTerrain = CManager::GetScene()->GetGameObject<CTerrain>(CManager::LAYER_BACKGROUND);

	Vector3 push_Vec;
	if (pTerrain->GetCollision(m_CollisionBody[6]->GetSphere(), push_Vec)) {
		m_Position += Vector3(push_Vec.x, 0.0f, push_Vec.z);
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

void CPlayer::SetMotionStop(const bool _stop)
{
	m_pModel->StopMotion(_stop);
}

void CPlayer::UseItem()
{
	m_Item->Use();
}

void CPlayer::ChangeState(CStatePlayer* pState)
{
	delete m_pState;
	m_pState = pState;
}

void CPlayer::ReSpwan()
{
	// ステータス初期化
	m_Status.Life = 100;
	m_Status.Stamina = 100;

	// トランスフォーム初期化
	m_Position = m_SPAWN_POSITION;
	m_Rotation = Vector3(0.0f, 0.0f, 0.0f);
	m_Scale = Vector3(1.0f, 1.0f, 1.0f);

	// Front_Up_Rightベクトル初期化
	m_DirVec.SetFrontUpRight(Vector3(0.0f, 0.0f, 1.0f));

	// 納刀状態にする
	m_WeaponState = SWORD_STATE_SHEATHE;

	// 各フラグの初期化
	m_Collision = false;
	m_StateFlags.Idle = false;
	m_StateFlags.Damage = false;
	m_StateFlags.Dodge = false;
	m_StateFlags.Attack = false;
	m_StateFlags.Block = false;
	m_StateFlags.Dash = false;
	m_StateFlags.Move = false;
	m_StateFlags.Died = false;
	m_StateFlags.StaminaEmpty = false;
}
