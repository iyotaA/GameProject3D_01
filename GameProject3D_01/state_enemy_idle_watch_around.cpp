#include "game_objects_all.h"
#include "state_enemy_idle_watch_around.h"
#include "modelAnimation.h"
#include "enemy.h"
#include "MathFunc.h"

CStateEnemyIdleWatchAround::CStateEnemyIdleWatchAround(CEnemy* pEnemy)
	: m_LerpStart(CCameraManager::GetCamera()->GetLengthToAt())
	, m_LerpEnd(4.0f)
	, m_FrameCounter(0.0f)
	, m_MoveSpeed(1.0f)
	, m_Volocity(Vector3())
{
	pEnemy->SetAnimation(ENEMY_STATE_IDLE_WATCH_AROUND, 1.0f);
	pEnemy->SetAnimationSpeed(1.0f);
}

CStateEnemyIdleWatchAround::~CStateEnemyIdleWatchAround()
{

}

void CStateEnemyIdleWatchAround::UpdateIdleState(CStateEnemyIdle* pIdleState, CEnemy* pEnemy)
{
	// 入力・状態遷移
	Action(pEnemy);

	// 移動処理
	Move(pEnemy);

	// カウンター更新
	m_FrameCounter++;
}

void CStateEnemyIdleWatchAround::Move(CEnemy* pEnemy)
{
}

void CStateEnemyIdleWatchAround::Action(CEnemy* pEnemy)
{
}

