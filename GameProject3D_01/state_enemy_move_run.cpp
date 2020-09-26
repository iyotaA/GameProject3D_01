#include "game_objects_all.h"
#include "scene.h"
#include "state_enemy_move_run.h"
#include "state_enemy_move_rotate.h"
#include "state_enemy_idle.h"
#include "enemy.h"
#include "MathFunc.h"


CStateEnemyMoveRun::CStateEnemyMoveRun(CEnemy* pEnemy)
	: m_FrameCounter(0)
	, m_PlayerAccesser(CManager::GetScene()->GetGameObject<CPlayer>(CManager::LAYER_OBJECT))
{
	pEnemy->SetAnimation(ENEMY_STATE_RUN, 1.0f);
	pEnemy->SetAnimationSpeed(2.5f);

	// プレイヤーへの向きを算出
	Vector3 direction = *m_PlayerAccesser->GetPosition() - *pEnemy->GetPosition();
	direction.Normalize();

	// 向きと速さから速度を算出
	m_MoveSpeed = direction * 0.45f;

	// 攻撃を有効状態に
	pEnemy->Attacked() = true;

	// 攻撃力設定
	pEnemy->Attack() = 35;
}

CStateEnemyMoveRun::~CStateEnemyMoveRun()
{

}

void CStateEnemyMoveRun::UpdateMoveState(CStateEnemyMove* pMoveState, CEnemy* pEnemy)
{
	// 移動処理
	Move(pEnemy);

	// プレイヤーとの距離算出
	Vector3 distance = *m_PlayerAccesser->GetPosition() - *pEnemy->GetPosition();

	//=====================================================
	//		遷移条件
	//=====================================================
	if ((m_FrameCounter >= 180) ||		// 180F後に遷移
		((pEnemy->CollisionTerrian()) && m_FrameCounter >= 50) ||		// 50F後以降に地形に衝突したら遷移
		((distance.Length() >= 20.0f) && m_FrameCounter >= 100)) {	// 100F後以降にプレイヤーとの距離が30離れたら遷移

		pMoveState->ChangeState(new CStateEnemyMoveRotate(pEnemy));
		pEnemy->Attacked() = false;// 攻撃を無効状態に
		return;
	}

	// カウンター更新
	m_FrameCounter++;
}

bool CStateEnemyMoveRun::Move(CEnemy* pEnemy)
{
	// 更新前の位置取得
	Vector3 prev_pos = *pEnemy->GetPosition();

	// 移動スピードを足していく
	pEnemy->AddVelocity(m_MoveSpeed);

	// 更新前との移動量から進行方向を取得・セット
	Vector3 move_distance= *pEnemy->GetPosition() - prev_pos;
	pEnemy->Rotation().y = atan2f(move_distance.x, move_distance.z);

	// 三秒経ったら終了
	return (m_FrameCounter >= 180) ? true : false;
}
