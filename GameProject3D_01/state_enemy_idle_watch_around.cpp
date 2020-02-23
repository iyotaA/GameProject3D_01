#include "game_objects_all.h"
#include "state_enemy_idle_watch_around.h"
#include "state_enemy_idle_stop.h"
#include "state_enemy_move.h"
#include "enemy.h"
#include "MathFunc.h"

CStateEnemyIdleWatchAround::CStateEnemyIdleWatchAround(CEnemy* pEnemy)
	: m_FrameCounter(0)
{
	pEnemy->SetAnimation(ENEMY_STATE_IDLE_WATCH_AROUND, 1.0f);
	pEnemy->SetAnimationSpeed(1.0f);
}

CStateEnemyIdleWatchAround::~CStateEnemyIdleWatchAround()
{

}

void CStateEnemyIdleWatchAround::UpdateIdleState(CStateEnemyIdle* pIdleState, CEnemy* pEnemy)
{
	//--------------------------------------------------------------------------
	// ���G�\�͈͓��Ƀv���C���[�����邩�H
	if (pEnemy->InScoutingArea()) {
		pEnemy->ChangeState(new CStateEnemyMove(pEnemy, STATE_ROTATE));
		return;
	}

	if (m_FrameCounter++ < pEnemy->GetCurrentAnimFrameNum() - 1) return;


	/// �������n����ԂɑJ��
	pIdleState->ChangeState(new CStateEnemyIdleStop(pEnemy));
}

void CStateEnemyIdleWatchAround::Move(CEnemy* pEnemy)
{
}

void CStateEnemyIdleWatchAround::Action(CEnemy* pEnemy)
{
}

