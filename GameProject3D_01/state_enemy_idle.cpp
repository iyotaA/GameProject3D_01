#include "game_objects_all.h"
#include "state_enemy_idle.h"
#include "state_enemy_idle_stop.h"
#include "state_enemy_died.h"
#include "enemy.h"
#include "MathFunc.h"

CStateEnemyIdle::CStateEnemyIdle(CEnemy* pEnemy)
	: m_pStateIdle( new CStateEnemyIdleStop(pEnemy))
	, m_FrameCounter(0)
{
	pEnemy->SetMotionStop(false);
}

CStateEnemyIdle::~CStateEnemyIdle()
{

}

void CStateEnemyIdle::Update(CEnemy* pEnemy)
{
	// 死亡状態に遷移
	if (pEnemy->Died()) {
		pEnemy->ChangeState(new CStateEnemyDied(pEnemy));
		return;
	}

	m_pStateIdle->UpdateIdleState(this, pEnemy);

	// カウンター更新
	m_FrameCounter++;
}

void CStateEnemyIdle::ChangeState(CStateEnemyIdle* pIdleState)
{
	delete m_pStateIdle;
	m_pStateIdle = pIdleState;
}
