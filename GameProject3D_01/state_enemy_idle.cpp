#include "game_objects_all.h"
#include "state_enemy_idle.h"
#include "state_enemy_idle_stop.h"
#include "enemy.h"
#include "MathFunc.h"

CStateEnemyIdle::CStateEnemyIdle(CEnemy* pEnemy)
	: m_pStateIdle( new CStateEnemyIdleStop(pEnemy))
	, m_FrameCounter(0)
{
}

CStateEnemyIdle::~CStateEnemyIdle()
{

}

void CStateEnemyIdle::Update(CEnemy* pEnemy)
{
	m_pStateIdle->UpdateIdleState(this, pEnemy);

	// カウンター更新
	m_FrameCounter++;
}
