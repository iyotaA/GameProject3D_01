
// インクルード ////////////////////////////////////
#include "game_objects_all.h"
#include "scene.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "tutorial.h"

#define Glavity (0.098f)

void CEnemy::Init()
{
	// モデルの初期化
	m_pModel = new CModel;
	m_pModel->Load("asset/miku_01.obj");

	// トランスフォーム初期化
	m_Position = Vector3(0.0f, 1.0f, 5.0f);
	m_Rotation = Vector3(0.0f, XMConvertToRadians(180.0f), 0.0f);
	m_Scale    = Vector3(1.0f, 1.0f, 1.0f);

	// コリジョンの初期化
	m_CollisionSphere = new CCollisionSphere;
	m_CollisionSphere->SetRadius(0.7f);

	// ダメージ関連ステータスの初期化
	m_DamageManager = new CDamage(100, 15);
}

void CEnemy::Uninit()
{
	// モデルの終了処理
	m_pModel->Unload();
	delete m_pModel;
}

void CEnemy::Update()
{
	// 生きているか
	if (!m_DamageManager->IsAlive()) {
		CManager::GetScene()->DestroyGameObject(this);
		CManager::AddScore();
		return;
	}

	// コリジョン位置の更新
	m_CollisionSphere->SetCenter(&m_Position);

	m_Position.y -= Glavity;

	// 地面とのコリジョン
	CField* pField = CManager::GetScene()->GetGameObject<CField>(CManager::E_Background);
	float height = pField->GetHeight(&m_Position);
	if (FAILD_NUM != (int)height) {
		if (m_Position.y - m_CollisionSphere->GetRadius() <= height) {
			m_Position.y = height + m_CollisionSphere->GetRadius();
		}
	}
}

void CEnemy::Draw()
{
	ImGui::Begin("Enemy");
	m_DamageManager->DebugDraw();
	ImGui::End();

	// マトリクス設定
	XMMATRIX world;
	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	world *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

	m_pModel->Draw(&world);

	// デバッググリッドセット
	XMFLOAT4 color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	CDebugPrimitive::DebugPrimitive_BatchCirecleDraw(m_CollisionSphere, &color);
}
