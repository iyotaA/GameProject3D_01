#include "game_objects_all.h"
#include "state_enemy_move.h"
#include "state_enemy_move_run.h"
#include "state_enemy_move_walk.h"
#include "state_enemy_move_rotate.h"
#include "state_enemy_damage.h"
#include "state_enemy_idle.h"
#include "state_enemy_died.h"
#include "enemy.h"
#include "MathFunc.h"

CStateEnemyMove::CStateEnemyMove(CEnemy* pEnemy, ENEMY_STATE_MOVE state_move)
{
	switch (state_move)
	{
	case STATE_ROTATE:
		m_pStateMove = new CStateEnemyMoveRotate(pEnemy);
		break;
	case STATE_RUN:
		m_pStateMove = new CStateEnemyMoveRun(pEnemy);
		break;
	case STATE_WALK:
		m_pStateMove = new CStateEnemyMoveWalk(pEnemy);
		break;
	default:
		m_pStateMove = new CStateEnemyMoveNone(pEnemy);
		break;
	}
}

CStateEnemyMove::~CStateEnemyMove()
{
	delete m_pStateMove;
}

void CStateEnemyMove::Update(CEnemy* pEnemy)
{
	// 死亡状態に遷移
	if (pEnemy->Died()) {
		pEnemy->ChangeState(new CStateEnemyDied(pEnemy));
		return;
	}

	// 移動ステートが無ければ待機ステートに遷移
	if (typeid(*m_pStateMove) == typeid(CStateEnemyMoveNone)) {
		pEnemy->ChangeState(new CStateEnemyIdle(pEnemy));
	}

	// ダメージステートに遷移
	if (pEnemy->Damaged()) {
		pEnemy->ChangeState(new CStateEnemyDamage(pEnemy));
		return;
	}

	// 更新
	m_pStateMove->UpdateMoveState(this, pEnemy);

	// カウンター更新
	m_FrameCounter++;
}

bool CStateEnemyMove::Action(CEnemy* pEnemy)
{
	return true;
}

void CStateEnemyMove::ChangeState(CStateEnemyMove* pMoveState)
{
	delete m_pStateMove;
	m_pStateMove = pMoveState;
}
